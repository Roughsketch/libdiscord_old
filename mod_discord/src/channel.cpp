#include <thread>

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
    m_type = Text;
    m_position = 0;
    m_is_private = false;
    m_bitrate = 0;
    m_user_limit = 0;
    m_is_dm = false;
  }

  Channel::Channel(const nlohmann::json& data)
  {
    set_id_from_json("id", data);
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

  void Channel::merge(std::shared_ptr<Channel> other)
  {
    m_is_private = other->m_is_private;
    m_last_message_id = other->m_last_message_id;
    m_guild_id = other->m_guild_id;
    m_name = other->m_name;
    m_type = other->m_type;
    m_position = other->m_position;
    m_permission_overwrites = other->m_permission_overwrites;
    m_topic = other->m_topic;
    m_bitrate = other->m_bitrate;
    m_user_limit = other->m_user_limit;
    m_recipient = other->m_recipient;
    m_is_dm = other->m_is_dm;
  }

  bool Channel::is_dm() const
  {
    return m_is_dm;
  }

  bool Channel::is_pm() const
  {
    return m_is_dm;
  }

  Snowflake Channel::guild_id() const
  {
    return m_guild_id;
  }

  std::shared_ptr<Guild> Channel::guild() const
  {
    return ModDiscord::API::Guild::get_guild(guild_id());
  }

  std::string Channel::name() const
  {
    return m_name;
  }

  ChannelType Channel::type() const
  {
    return m_type;
  }

  uint32_t Channel::position() const
  {
    return m_position;
  }

  std::string Channel::topic() const
  {
    return m_topic;
  }

  uint32_t Channel::bitrate() const
  {
    return m_bitrate;
  }

  uint32_t Channel::user_limit() const
  {
    return m_user_limit;
  }

  void Channel::set_name(std::string name)
  {
    if (name.size() >= MinNameSize && name.size() <= MaxNameSize)
    {
      m_name = name;
    }
  }

  void Channel::set_topic(std::string topic)
  {
    if (topic.size() <= MaxTopicSize)
    {
      m_topic = topic;
    }
  }

  void Channel::set_position(uint32_t position)
  {
    m_position = position;
  }

  void Channel::set_bitrate(uint32_t bitrate)
  {
    //  TODO: Figure out how to determine whether a server is VIP and up the max to 128000
    if (bitrate < MinBitRate)
    {
      m_bitrate = MinBitRate;
    }
    else if (bitrate > MaxBitRate)
    {
      m_bitrate = MaxBitRate;
    }
    else
    {
      m_bitrate = bitrate;
    }
  }

  void Channel::set_user_limit(uint32_t user_limit)
  {
    if (user_limit <= MaxUserLimit)
    {
      m_user_limit = user_limit;
    }
  }


  std::shared_ptr<Channel> Channel::modify(std::function<void(std::shared_ptr<Channel>)> modify_block)
  {
    auto channel = ModDiscord::API::Channel::get_channel(id());
    modify_block(channel);
    if (channel->type() == Text)
    {
      channel = ModDiscord::API::Channel::modify_text_channel(channel->id(), channel->name(), channel->position(), channel->topic());
    }
    else if (channel->type() == Voice)
    {
      channel = ModDiscord::API::Channel::modify_voice_channel(channel->id(), channel->name(), channel->position(), channel->bitrate(), channel->user_limit());
    }
    return channel;
  }


  std::shared_ptr<Message> Channel::send_message(std::string content, bool tts) const
  {
    return ModDiscord::API::Channel::create_message(m_id, content);
  }

  void Channel::send_temp_message(std::string content, uint32_t timeout, bool tts) const
  {
    using namespace std::chrono;
    auto message = ModDiscord::API::Channel::create_message(m_id, content);
    std::this_thread::sleep_for(10s);
    message->remove();
  }
}
