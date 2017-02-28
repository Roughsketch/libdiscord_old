#pragma once

#include <cpprest/ws_client.h>

#include "common.h"

namespace ModDiscord
{
  class Bot;

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

  class Gateway
  {
    static const uint8_t LARGE_SERVER;
    static const utility::string_t VERSION;
    static const utility::string_t ENCODING;

    std::string m_token;
    std::string m_wss_url;
    web::websockets::client::websocket_callback_client m_client;
    std::mutex m_client_mutex;

    std::thread m_heartbeat_thread;
    uint32_t m_heartbeat_interval;
    bool m_recieved_ack;

    uint32_t m_last_seq;
    std::string m_session_id;

    std::weak_ptr<Bot> m_bot;
  public:
    Gateway();
    explicit Gateway(std::string token);

    void set_bot(std::weak_ptr<Bot> bot);
    void start();

    void on_message(web::websockets::client::websocket_incoming_message);

    void handle_dispatch_event(std::string event_name, nlohmann::json data);

    void send(Opcodes op, nlohmann::json packet);
    void send_heartbeat();
    void send_identify();
    void send_resume();
  };
}
