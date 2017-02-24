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
  }

  Gateway::Gateway(std::string token) : Gateway()
  {
    m_token = token;

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

    if (msg.message_type() == web::websockets::client::websocket_message_type::binary_message)
    {
      boost::iostreams::filtering_ostream os;

      os.push(boost::iostreams::zlib_decompressor());
      os.push(std::back_inserter(str));

      Concurrency::streams::container_buffer<std::string> strbuf;
      std::string compressed;

      compressed = msg.body().read_to_end(strbuf).then([strbuf](size_t bytesRead)
      {
        return strbuf.collection();
      }).get();

      boost::iostreams::write(os, compressed.data(), msg.length());
    }
    else
    {
      str = msg.extract_string().get();
    }

    auto payload = nlohmann::json::parse(str.c_str());

    BOOST_LOG_TRIVIAL(info) << "Got WS Payload: " << str.c_str();

    auto data = payload["d"];
    switch (payload["op"].get<uint8_t>())
    {
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
      BOOST_LOG_TRIVIAL(info) << "Recieved Heartbeat ACK.";
      break;
    default:
      BOOST_LOG_TRIVIAL(warning) << "Unhandled WS Opcode (" << static_cast<int>(payload["op"].get<uint8_t>()) << ")";
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
    BOOST_LOG_TRIVIAL(debug) << "Sending heartbeat packet.";
    send(Opcodes::Heartbeat, { m_last_seq });
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
}
