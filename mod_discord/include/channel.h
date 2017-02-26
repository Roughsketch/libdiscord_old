#pragma once

#include "common.h"
#include "identifiable.h"
#include "message.h"
#include "user.h"

namespace ModDiscord
{
  class Overwrite
  {
    snowflake m_id;
    std::string m_type;
    uint32_t m_allow;
    uint32_t m_deny;
  public:
    Overwrite();
    explicit Overwrite(const nlohmann::json& data);
  };

  inline void from_json(const nlohmann::json& json, Overwrite& overwrite)
  {
    overwrite = Overwrite(json);
  }

  enum ChannelType : uint8_t
  {
    Text = 0,
    Private,
    Voice,
    Group
  };

  class Channel : public Identifiable
  {
    //  All Channels
    bool m_is_private;
    snowflake m_last_message_id;

    //  Guild Channels
    snowflake m_guild_id;
    std::string m_name;
    ChannelType m_type;
    uint32_t m_position;
    std::vector<Overwrite> m_permission_overwrites;
    std::string m_topic;
    uint32_t m_bitrate;
    uint32_t m_user_limit;

    //  DM Specific
    User m_recipient;

    bool m_is_dm;
  public:
    Channel();
    explicit Channel(const nlohmann::json& data);

    bool is_dm() const;
    bool is_pm() const;
    std::shared_ptr<Message> send_message(std::string content, bool tts = false) const;
    void Channel::send_temp_message(std::string content, uint32_t timeout, bool tts = false) const;
  };

  inline void from_json(const nlohmann::json& json, Channel& channel)
  {
    channel = Channel(json);
  }
}
