#pragma once

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <cpprest/ws_client.h>

#include "api.h"

namespace ModDiscord
{
  enum Opcodes : uint8_t
  {
    Dispatch = 0,
    Heartbeat,
    Identify,
    Presence,
    Voice_State,
    Voice_Ping,
    Resume,
    Reconnect,
    Request_Members,
    Invalidate_Session,
    Hello,
    Heartbeat_ACK
  };

  class Session
  {
    
  };

  class Gateway
  {
    static const uint8_t LARGE_SERVER;
    static const utility::string_t VERSION;
    static const utility::string_t ENCODING;

    std::string m_token;
    std::string m_wss_url;
    web::websockets::client::websocket_callback_client m_client;
    std::mutex m_client_mutex;

    uint32_t m_heartbeat_interval;
    uint32_t m_last_seq;

    std::thread m_heartbeat_thread;
  public:
    Gateway();
    explicit Gateway(std::string token);

    void on_message(web::websockets::client::websocket_incoming_message);
    void send(Opcodes op, nlohmann::json packet);
    void send_heartbeat();
    void send_identify();
  };
}
