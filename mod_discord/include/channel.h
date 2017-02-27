#pragma once

#include "common.h"
#include "identifiable.h"
#include "message.h"
#include "user.h"

namespace ModDiscord
{
  class Overwrite
  {
    Snowflake m_id;
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
    static const uint32_t MinNameSize = 2;
    static const uint32_t MaxNameSize = 100;
    static const uint32_t MaxTopicSize = 1024;
    static const uint32_t MinBitRate = 8000;
    static const uint32_t MaxBitRate = 96000;
    static const uint32_t MaxVIPBitRate = 128000;
    static const uint32_t MaxUserLimit = 99;

    //  All Channels
    bool m_is_private;
    Snowflake m_last_message_id;

    //  Guild Channels
    Snowflake m_guild_id;
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

    void merge(std::shared_ptr<Channel> other);

    bool is_dm() const;
    bool is_pm() const;

    Snowflake guild_id() const;
    std::string name() const;
    ChannelType type() const;
    uint32_t position() const;
    std::string topic() const;
    uint32_t bitrate() const;
    uint32_t user_limit() const;

    void set_name(std::string name);
    void set_topic(std::string topic);
    void set_position(uint32_t position);
    void set_bitrate(uint32_t bitrate);
    void set_user_limit(uint32_t user_limit);

    std::shared_ptr<Channel> modify(std::function<void(std::shared_ptr<Channel>)> modify_block);
    std::shared_ptr<Message> send_message(std::string content, bool tts = false) const;
    void Channel::send_temp_message(std::string content, uint32_t timeout, bool tts = false) const;
  };

  inline void from_json(const nlohmann::json& json, Channel& channel)
  {
    channel = Channel(json);
  }
}
