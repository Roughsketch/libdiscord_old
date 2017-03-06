#pragma once

#include <cpprest/ws_client.h>
#include <thread>

#include "common.h"

namespace Discord
{
  class Bot;

  class Gateway
  {
    //  Constants
    static const uint8_t LARGE_SERVER;
    static const utility::string_t VERSION;
    static const utility::string_t ENCODING;

    //  Client variables
    std::string m_token;
    utility::string_t m_wss_url;
    web::websockets::client::websocket_callback_client m_client;
    std::mutex m_client_mutex;

    //  Heartbeat variables
    std::thread m_heartbeat_thread;
    uint32_t m_heartbeat_interval;
    bool m_recieved_ack;

    //  Session variables
    uint32_t m_last_seq;
    std::string m_session_id;
    volatile bool m_connected;
    bool m_use_resume;

    //  Owning bot
    std::weak_ptr<Bot> m_bot;

    //  Private enumeration for Opcodes
    enum Opcode : uint8_t
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

    //  Private methods
    void connect();
    void on_message(web::websockets::client::websocket_incoming_message);
    void handle_dispatch_event(std::string event_name, nlohmann::json data);
    void send(Opcode op, nlohmann::json packet);
    void send_heartbeat();
    void send_identify();
    void send_resume();
  public:
    Gateway();
    explicit Gateway(std::string token);

    /** Sets the bot that this gateway will call for events.
     
        @param bot A shared_ptr to the bot.
     */
    void set_bot(std::weak_ptr<Bot> bot);

    /** Start a gateway connection. */
    void start();

    /** Whether or not this gateway is currently connected.
     
        @return Connection status.
     */
    bool connected() const;
  };
}
