#pragma once
#include <cstdint>
#include <string>


namespace ModDiscord
{
  namespace detail
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
  }

  class Session
  {
    
  };

  class Gateway
  {
    static const uint8_t LARGE_SERVER;
    static const std::string VERSION;

    std::string m_token;

  public:
    explicit Gateway(std::string token);
  };
}
