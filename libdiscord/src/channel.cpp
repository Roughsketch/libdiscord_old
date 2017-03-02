#include <thread>

#include "api.h"
#include "channel.h"
#include "message.h"
#include "user.h"
#include "guild.h"

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

  std::string Overwrite::type() const
  {
    return m_type;
  }

  std::shared_ptr<Permission> Overwrite::allow() const
  {
    return m_allow;
  }

  std::shared_ptr<Permission> Overwrite::deny() const
  {
    return m_deny;
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

  std::shared_ptr<Channel> Channel::close() const
  {
    return ModDiscord::API::Channel::delete_channel(m_id);
  }

  std::vector<std::shared_ptr<Message>> Channel::get_messages(int32_t limit, SearchCriteria method, Snowflake message_id) const
  {
    if (method != None && message_id == 0)
    {
      //  Do not pass search methods if no message id was passed.
      return ModDiscord::API::Channel::get_messages(m_id, limit);
    }

    return ModDiscord::API::Channel::get_messages(m_id, limit, method, message_id);
  }

  std::shared_ptr<Message> Channel::get_message(Snowflake message_id) const
  {
    return ModDiscord::API::Channel::get_message(m_id, message_id);
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

  bool Channel::add_reaction(Snowflake message_id, std::shared_ptr<Emoji> emoji) const
  {
    return ModDiscord::API::Channel::create_reaction(m_id, message_id, emoji);
  }

  bool Channel::remove_reaction(Snowflake message_id, std::shared_ptr<Emoji> emoji, Snowflake user_id) const
  {
    if (user_id == 0)
    {
      //  If no user was specified, assume it's our own reaction
      return ModDiscord::API::Channel::delete_own_reaction(m_id, message_id, emoji);
    }

    return ModDiscord::API::Channel::delete_user_reaction(m_id, message_id, emoji, user_id);
  }

  std::vector<std::shared_ptr<User>> Channel::get_reactions(Snowflake message_id, std::shared_ptr<Emoji> emoji) const
  {
    return ModDiscord::API::Channel::get_reactions(m_id, message_id, emoji);
  }

  void Channel::remove_all_reactions(Snowflake message_id) const
  {
    ModDiscord::API::Channel::delete_all_reactions(m_id, message_id);
  }

  std::shared_ptr<Message> Channel::edit_message(Snowflake message_id, std::string new_content) const
  {
    return ModDiscord::API::Channel::edit_message(m_id, message_id, new_content);
  }

  bool Channel::remove_message(Snowflake message_id) const
  {
    return ModDiscord::API::Channel::delete_message(m_id, message_id);
  }

  bool Channel::remove_messages(std::vector<Snowflake> message_ids) const
  {
    //  Can only currently bulk delete between 2 and 100 messages.
    if (message_ids.size() < 2 || message_ids.size() > 100)
    {
      return false;
    }

    return ModDiscord::API::Channel::bulk_delete_messages(m_id, message_ids);
  }

  bool Channel::edit_permissions(std::shared_ptr<Overwrite> overwrite, 
    std::function<void(std::shared_ptr<Permission>, std::shared_ptr<Permission>)> callback) const
  {
    callback(overwrite->allow(), overwrite->deny());
    return ModDiscord::API::Channel::edit_permissions(m_id, overwrite,
              overwrite->allow()->get(), overwrite->deny()->get(), overwrite->type());
  }

  bool Channel::edit_permissions(std::shared_ptr<Overwrite> overwrite, Permission allow, Permission deny) const
  {
    return ModDiscord::API::Channel::edit_permissions(m_id, overwrite, allow.get(), deny.get(), overwrite->type());
  }

  std::vector<std::shared_ptr<Invite>> Channel::get_invites() const
  {
    return ModDiscord::API::Channel::get_channel_invites(m_id);
  }

  std::shared_ptr<Invite> Channel::create_invite(uint32_t max_age, uint32_t max_uses, bool temporary, bool unique) const
  {
    return ModDiscord::API::Channel::create_channel_invite(m_id, max_age, max_uses, temporary, unique);
  }

  bool Channel::delete_permission(std::shared_ptr<Overwrite> overwrite) const
  {
    return ModDiscord::API::Channel::delete_permission(m_id, overwrite);
  }

  bool Channel::start_typing() const
  {
    return ModDiscord::API::Channel::trigger_typing_indicator(m_id);
  }

  std::vector<std::shared_ptr<Message>> Channel::get_pinned() const
  {
    return ModDiscord::API::Channel::get_pinned_messages(m_id);
  }

  bool Channel::add_pin(Snowflake message_id) const
  {
    return ModDiscord::API::Channel::add_pinned_message(m_id, message_id);
  }

  bool Channel::remove_pin(Snowflake message_id) const
  {
    return ModDiscord::API::Channel::delete_pinned_message(m_id, message_id);
  }
}
