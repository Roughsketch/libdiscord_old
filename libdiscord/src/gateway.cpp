#include "gateway.h"

#include "api.h"
#include "bot.h"

#include <zlib.h>
#include <cpprest/http_msg.h>

namespace Discord
{
  const uint8_t Gateway::LARGE_SERVER = 100;
  const utility::string_t Gateway::VERSION = utility::string_t(U("6"));
  const utility::string_t Gateway::ENCODING = utility::string_t(U("json"));

  Gateway::Gateway()
  {
    m_heartbeat_interval = 0;
    m_last_seq = 0;
    m_recieved_ack = true; // Set true to start because first hearbeat sent doesn't require an ACK.
    m_connected = false;
    m_use_resume = false;
  }

  Gateway::Gateway(std::string token) : Gateway()
  {
    m_token = token;
  }

  void Gateway::set_bot(std::weak_ptr<Bot> bot)
  {
    m_bot = bot;
  }

  void Gateway::start()
  {
    web::uri_builder builder(U(""));
    builder.append_query(U("v"), VERSION);
    builder.append_query(U("encoding"), ENCODING);

    if (m_wss_url.empty())
    {
      do
      {
        try
        {
          //  Attempt to get the Gateway URL.
          m_wss_url = utility::conversions::to_string_t(API::get_wss_url());
        }
        catch (const web::http::http_exception& e)
        {
          LOG(ERROR) << "Exception getting gateway URL: " << e.what();
          LOG(ERROR) << "Sleeping for 5 seconds and trying again.";
          std::this_thread::sleep_for(std::chrono::seconds(5));
        }
      } while (m_wss_url.empty());  //  Keep trying until we get it.

      m_wss_url += builder.to_string();
    }

    m_client.set_message_handler([&](web::websockets::client::websocket_incoming_message msg)
    {
      try 
      {
        on_message(msg);
      }
      catch (const std::exception& e)
      {
        LOG(ERROR) << "WebSocket Exception: " << e.what();
      }
    });

    m_client.set_close_handler([&](web::websockets::client::websocket_close_status status, const utility::string_t& reason, const std::error_code& code)
    {
      if (m_connected)
      {
        LOG(ERROR)  << "WebSocket connection has closed with reason "
                    << utility::conversions::to_utf8string(reason) << " - "
                    << code.message() << " (" << code.value() << ")";
        m_connected = false;
        connect();
      }
    });

    connect();
  }

  void Gateway::connect()
  {
    LOG(DEBUG) << "Connecting to " << utility::conversions::to_utf8string(m_wss_url);

    while (!m_connected)
    {
      //  Try to connect
      m_client.connect(m_wss_url).then([](){}).get();

      //  Sleep 5 seconds before checking connected status.
      std::this_thread::sleep_for(std::chrono::seconds(5));

      if (!m_connected)
      {
        LOG(DEBUG) << "Took longer than 5 seconds to receive a HELLO response, trying to connect again.";
      }
    }
  }

  void Gateway::on_message(web::websockets::client::websocket_incoming_message msg)
  {
    std::string str;

    //  If the message is binary data, then we need to decompress it using ZLib.
    if (msg.message_type() == web::websockets::client::websocket_message_type::binary_message)
    {
      Concurrency::streams::container_buffer<std::string> strbuf;
      std::string compressed;

      //  Read the entire binary payload and put into a string container
      compressed = msg.body().read_to_end(strbuf).then([strbuf](size_t bytesRead)
      {
        return strbuf.collection();
      }).get();

      z_stream zs;
      memset(&zs, 0, sizeof(zs));

      if (inflateInit(&zs) != Z_OK)
      {
        LOG(ERROR) << "Could not initialize zlib Inflate";
      }

      zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(compressed.data()));
      zs.avail_in = static_cast<uInt>(compressed.size());

      int ret;
      char buffer[32768];

      do
      {
        zs.next_out = reinterpret_cast<Bytef *>(buffer);
        zs.avail_out = sizeof(buffer);

        ret = inflate(&zs, 0);

        if (str.size() < zs.total_out)
        {
          str.append(buffer, zs.total_out - str.size());
        }
      } while (ret == Z_OK);

      inflateEnd(&zs);

      if (ret != Z_STREAM_END)
      {
        LOG(ERROR) << "Error during zlib decompression: (" << ret << ")";
      }
    }
    else
    {
      //  If not compressed, just get the string.
      str = msg.extract_string().get();
    }

    //  Parse our payload as JSON.
    auto payload = nlohmann::json::parse(str.c_str());

    if (str.size() > 1000)
    {
      LOG(DEBUG) << "Got WS Payload: " << payload.dump(2).substr(0, 1000);
    }
    else
    {
      LOG(DEBUG) << "Got WS Payload: " << payload.dump(2);
    }

    auto data = payload["d"]; //  Get the data for the event

    switch (payload["op"].get<uint8_t>())
    {
    case Dispatch:
      m_last_seq = payload["s"];
      handle_dispatch_event(payload["t"], data);
      break;
    case Reconnect:
      send_resume();
      break;
    case Hello:
      m_connected = true;
      m_heartbeat_interval = data["heartbeat_interval"].get<uint32_t>();
      LOG(DEBUG) << "Set heartbeat interval to " << m_heartbeat_interval;

      if (m_use_resume)
      {
        LOG(INFO) << "Connected again, sending Resume packet.";
        send_resume();
      }
      else
      {
        m_heartbeat_thread = std::thread([&]() {
          while (connected())
          {
            send_heartbeat();
            std::this_thread::sleep_for(std::chrono::milliseconds(m_heartbeat_interval));
          }

          LOG(DEBUG) << "Disconnected, stopping heartbeats.";
        });

        LOG(DEBUG) << "Connected, sending Identify packet.";
        send_identify();
        m_use_resume = true;  //  Next time use Resume
      }
      break;
    case Heartbeat_ACK:
      LOG(TRACE) << "Recieved Heartbeat ACK.";
      m_recieved_ack = true;
      break;
    default:
      LOG(WARNING) << "Unhandled WS Opcode (" << static_cast<int>(payload["op"].get<uint8_t>()) << ")";
    }
  }

  void Gateway::handle_dispatch_event(std::string event_name, nlohmann::json data)
  {
    //LOG(INFO) << "Recieved " << event_name << " event.";

    if (event_name == "READY")
    {
      LOG(DEBUG) << "Using gateway version " << data["v"];

      //  Save session id so we can restart a session
      m_session_id = data["session_id"].get<std::string>();

      if (auto p = m_bot.lock())
      {
        p->handle_dispatch(event_name, data);
      }
      else
      {
        LOG(ERROR) << "Could not lock Bot pointer.";
      }
    }
    else if (event_name == "RESUMED")
    {
      LOG(DEBUG) << "Successfully resumed.";
    }
    else
    {
      if (auto p = m_bot.lock())
      {
        p->handle_dispatch(event_name, data);
      }
      else
      {
        LOG(ERROR) << "Could not lock Bot pointer.";
      }
    }
  }

  void Gateway::send(Opcode op, nlohmann::json data)
  {
    nlohmann::json packet = {
      { "op", op },
      { "d", data }
    };

    web::websockets::client::websocket_outgoing_message msg;
    msg.set_utf8_message(packet.dump());

    LOG(DEBUG) << "Sending packet: " << packet.dump(2);

    try
    {
      m_client.send(msg);
    }
    catch(web::websockets::client::websocket_exception& e)
    {
      LOG(ERROR) << "WS Exception: " << e.what();
    }
  }

  void Gateway::send_heartbeat()
  {
    if (!m_recieved_ack)
    {
      LOG(WARNING) << "Did not recieve a heartbeat ACK packet before this heartbeat.";
    }

    LOG(DEBUG) << "Sending heartbeat packet.";
    send(Heartbeat, { m_last_seq });
    m_recieved_ack = false;
  }

  void Gateway::send_identify()
  {
    LOG(DEBUG) << "Sending identify packet.";

    send(Identify,
    {
      { "token", m_token },
      {
        "properties",
        {
          { "$os", "windows" },
          { "$browser", "Discord" },
          { "$device", "Discord" },
          { "$referrer", "" },
          { "$refferring_domain", "" }
        }
      },
      { "compress", true },
      { "large_threshold", LARGE_SERVER },
      { "shard", nlohmann::json::array({ 0, 1 }) }
    });
  }

  void Gateway::send_resume()
  {
    LOG(DEBUG) << "Sending resume packet.";

    send(Resume, 
    {
      { "token", m_token },
      { "session_id", m_session_id },
      { "seq", m_last_seq }
    });
  }

  bool Gateway::connected() const
  {
    return m_connected;
  }
}
