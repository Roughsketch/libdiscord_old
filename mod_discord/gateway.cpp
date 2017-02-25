#include "gateway.h"

namespace ModDiscord
{
  const uint8_t Gateway::LARGE_SERVER = 100;
  const utility::string_t Gateway::VERSION = L"6";
  const utility::string_t Gateway::ENCODING = L"json";

  Gateway::Gateway()
  {
    m_heartbeat_interval = 0;
    m_last_seq = 0;
    m_recieved_ack = true; // Set true to start because first hearbeat sent doesn't require an ACK.
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
    web::uri_builder builder(L"");
    builder.append_query(L"v", VERSION);
    builder.append_query(L"encoding", ENCODING);

    auto wss_url = utility::conversions::to_string_t(API::get_wss_url());
    wss_url += builder.to_string();

    BOOST_LOG_TRIVIAL(info) << "WSS URL: " << wss_url.c_str();

    std::lock_guard<std::mutex> lock(m_client_mutex);

    m_client.connect(wss_url).then([]() {
      BOOST_LOG_TRIVIAL(info) << "Gateway connected.";
    });

    m_client.set_message_handler([this](web::websockets::client::websocket_incoming_message msg)
    {
      this->on_message(msg);
    });
  }

  void Gateway::on_message(web::websockets::client::websocket_incoming_message msg)
  {
    std::string str;

    //  If the message is binary data, then we need to decompress it using ZLib.
    if (msg.message_type() == web::websockets::client::websocket_message_type::binary_message)
    {
      boost::iostreams::filtering_ostream os;

      os.push(boost::iostreams::zlib_decompressor()); //  Add the zlib decompressor
      os.push(std::back_inserter(str));               //  Set our decompressor to write to str

      Concurrency::streams::container_buffer<std::string> strbuf;
      std::string compressed;

      //  Read the entire binary payload and put into a string container
      compressed = msg.body().read_to_end(strbuf).then([strbuf](size_t bytesRead)
      {
        return strbuf.collection();
      }).get();

      //  Decompress using our iostream
      boost::iostreams::write(os, compressed.data(), msg.length());
    }
    else
    {
      //  If not compressed, just get the string.
      str = msg.extract_string().get();
    }

    //  Parse our payload as JSON.
    auto payload = nlohmann::json::parse(str.c_str());

    BOOST_LOG_TRIVIAL(info) << "Got WS Payload: " << payload.dump(2);

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
      m_heartbeat_interval = data["heartbeat_interval"].get<uint32_t>();
      BOOST_LOG_TRIVIAL(info) << "Set heartbeat interval to " << m_heartbeat_interval;

      m_heartbeat_thread = std::thread([this]() {
        for (;;)
        {
          send_heartbeat();
          std::this_thread::sleep_for(std::chrono::milliseconds(m_heartbeat_interval));
        }
      });

      send_identify();
      break;
    case Heartbeat_ACK:
      BOOST_LOG_TRIVIAL(trace) << "Recieved Heartbeat ACK.";
      m_recieved_ack = true;
      break;
    default:
      BOOST_LOG_TRIVIAL(warning) << "Unhandled WS Opcode (" << static_cast<int>(payload["op"].get<uint8_t>()) << ")";
    }
  }

  void Gateway::handle_dispatch_event(std::string event_name, nlohmann::json data)
  {
    BOOST_LOG_TRIVIAL(trace) << "Recieved " << event_name << " event.";

    if (event_name == "READY")
    {
      BOOST_LOG_TRIVIAL(info) << "Using gateway version " << data["v"];
      m_session_id = data["session_id"].get<std::string>();

      if (auto p = m_bot.lock())
      {
        p->handle_dispatch(event_name, data);
      }
      else
      {
        BOOST_LOG_TRIVIAL(error) << "Could not lock Bot pointer.";
      }
    }
    else if (event_name == "RESUMED")
    {
      BOOST_LOG_TRIVIAL(info) << "Successfully resumed.";
    }
    else
    {
      if (auto p = m_bot.lock())
      {
        p->handle_dispatch(event_name, data);
      }
      else
      {
        BOOST_LOG_TRIVIAL(error) << "Could not lock Bot pointer.";
      }
    }
  }

  void Gateway::send(Opcodes op, nlohmann::json data)
  {
    nlohmann::json packet = {
      { "op", op },
      { "d", data }
    };

    web::websockets::client::websocket_outgoing_message msg;
    msg.set_utf8_message(packet.dump());

    BOOST_LOG_TRIVIAL(debug) << "Sending packet: " << packet.dump(2);

    std::lock_guard<std::mutex> lock(m_client_mutex);
    m_client.send(msg);
  }

  void Gateway::send_heartbeat()
  {
    if (!m_recieved_ack)
    {
      BOOST_LOG_TRIVIAL(warning) << "Did not recieve a heartbeat ACK packet before this heartbeat.";
    }

    BOOST_LOG_TRIVIAL(debug) << "Sending heartbeat packet.";
    send(Opcodes::Heartbeat, { m_last_seq });
    m_recieved_ack = false;
  }

  void Gateway::send_identify()
  {
    BOOST_LOG_TRIVIAL(debug) << "Sending identify packet.";

    send(Opcodes::Identify,
    {
      { "token", m_token },
      {
        "properties",
        {
          { "$os", "windows" },
          { "$browser", "ModDiscord" },
          { "$device", "ModDiscord" },
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
    BOOST_LOG_TRIVIAL(debug) << "Sending resume packet.";

    send(Resume, 
    {
      { "token", m_token },
      { "session_id", m_session_id },
      { "seq", m_last_seq }
    });
  }
}
