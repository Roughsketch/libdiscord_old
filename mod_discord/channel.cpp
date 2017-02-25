#include "api.h"
#include "channel.h"

namespace ModDiscord
{
  Overwrite::Overwrite()
  {
    m_allow = false;
    m_deny = false;
  }

  Overwrite::Overwrite(const nlohmann::json& data)
  {
    set_from_json(m_id, "id", data);
    set_from_json(m_type, "type", data);
    set_from_json(m_allow, "allow", data);
    set_from_json(m_deny, "deny", data);
  }

  Channel::Channel()
  {
    m_id = 0;
    m_guild_id = 0;
    m_type = Text;
    m_position = 0;
    m_is_private = false;
    m_last_message_id = 0;
    m_bitrate = 0;
    m_user_limit = 0;
    m_is_dm = false;
  }

  Channel::Channel(const nlohmann::json& data)
  {
    set_from_json(m_id, "id", data);
    set_from_json(m_is_private, "is_private", data);
    set_from_json(m_last_message_id, "last_message_id", data);

    set_from_json(m_guild_id, "guild_id", data);
    set_from_json(m_name, "name", data);
    set_from_json(m_type, "type", data);
    set_from_json(m_position, "position", data);
    set_from_json(m_permission_overwrites, "permission_overwrites", data);
    set_from_json(m_topic, "topic", data);
    set_from_json(m_bitrate, "bitrate", data);
    set_from_json(m_user_limit, "user_limit", data);

    if (data.count("recipient"))
    {
      m_is_dm = true;
      set_from_json(m_recipient, "recipient", data);
    }
  }

  bool Channel::is_dm() const
  {
    return m_is_dm;
  }

  bool Channel::is_pm() const
  {
    return m_is_dm;
  }

  Event::Message Channel::send_message(std::string content, bool tts) const
  {
    auto response = ModDiscord::API::Channel::create_message(m_id, content);
    return Event::Message(response);
  }

  void Channel::send_temp_message(std::string content, uint32_t timeout, bool tts) const
  {
    using namespace std::chrono;
    auto response = ModDiscord::API::Channel::create_message(m_id, content);
    auto message = Event::Message(response);
    std::this_thread::sleep_for(10s);
  }
}
