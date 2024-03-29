#include <thread>

#include "api.h"
#include "api/api_channel.h"
#include "api/api_guild.h"
#include "channel.h"
#include "embed.h"
#include "message.h"
#include "user.h"
#include "guild.h"

namespace Discord
{
  Overwrite::Overwrite()
  {
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

  void Channel::set_guild_id(Snowflake guild_id)
  {
    m_guild_id = guild_id;
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
    return Discord::API::Guild::get(guild_id());
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


  std::shared_ptr<Channel> Channel::modify(std::function<void(std::shared_ptr<Channel>)> modify_block) const
  {
    auto channel = Discord::API::Channel::get(id());
    modify_block(channel);

    if (channel->type() == Text)
    {
      channel = Discord::API::Channel::modify_text_channel(channel->id(), channel->name(), channel->position(), channel->topic());
    }
    else if (channel->type() == Voice)
    {
      channel = Discord::API::Channel::modify_voice_channel(channel->id(), channel->name(), channel->position(), channel->bitrate(), channel->user_limit());
    }

    return channel;
  }

  std::shared_ptr<Channel> Channel::remove() const
  {
    return Discord::API::Channel::remove(m_id);
  }

  std::vector<std::shared_ptr<Message>> Channel::get_messages(int32_t limit, SearchCriteria method, Snowflake message_id) const
  {
    if (method != SearchCriteria::None && message_id == 0)
    {
      //  Do not pass search methods if no message id was passed.
      return Discord::API::Channel::get_messages(m_id, limit);
    }

    return Discord::API::Channel::get_messages(m_id, limit, method, message_id);
  }

  std::shared_ptr<Message> Channel::get_message(Snowflake message_id) const
  {
    return Discord::API::Channel::get_message(m_id, message_id);
  }

  std::shared_ptr<Message> Channel::send_message(std::string content, bool tts) const
  {
    return Discord::API::Channel::create_message(m_id, content, tts);
  }

  void Channel::send_temp_message(std::string content, uint32_t timeout, bool tts) const
  {
    using namespace std::chrono;
    auto message = Discord::API::Channel::create_message(m_id, content, tts);
    std::this_thread::sleep_for(10s);
    message->remove();
  }

  std::shared_ptr<Message> Channel::send_embed(Embed embed, std::string content, bool tts) const
  {
    return Discord::API::Channel::create_message(m_id, content, tts, std::make_shared<Embed>(embed));
  }

  bool Channel::add_reaction(Snowflake message_id, std::shared_ptr<Emoji> emoji) const
  {
    return Discord::API::Channel::create_reaction(m_id, message_id, emoji);
  }

  bool Channel::remove_reaction(Snowflake message_id, std::shared_ptr<Emoji> emoji, Snowflake user_id) const
  {
    if (user_id == 0)
    {
      //  If no user was specified, assume it's our own reaction
      return Discord::API::Channel::remove_own_reaction(m_id, message_id, emoji);
    }

    return Discord::API::Channel::remove_user_reaction(m_id, message_id, emoji, user_id);
  }

  std::vector<std::shared_ptr<User>> Channel::get_reactions(Snowflake message_id, std::shared_ptr<Emoji> emoji) const
  {
    return Discord::API::Channel::get_reactions(m_id, message_id, emoji);
  }

  void Channel::remove_all_reactions(Snowflake message_id) const
  {
    Discord::API::Channel::remove_all_reactions(m_id, message_id);
  }

  std::shared_ptr<Message> Channel::edit_message(Snowflake message_id, std::string new_content) const
  {
    return Discord::API::Channel::edit_message(m_id, message_id, new_content);
  }

  bool Channel::remove_message(Snowflake message_id) const
  {
    return Discord::API::Channel::remove_message(m_id, message_id);
  }

  bool Channel::remove_messages(std::vector<Snowflake> message_ids) const
  {
    //  Can only currently bulk delete between 2 and 100 messages.
    if (message_ids.size() < 2 || message_ids.size() > 100)
    {
      return false;
    }

    return Discord::API::Channel::bulk_remove_messages(m_id, message_ids);
  }

  bool Channel::prune(uint32_t amount)
  {
    if (amount < 2 || amount > 100)
    {
      throw DiscordException("Must prune between 2 and 100 messages, but got " + std::to_string(amount) + " as an argument.");
    }

    auto messages = Discord::API::Channel::get_messages(m_id, amount);
    std::vector<Snowflake> message_ids;

    std::transform(std::begin(messages), std::end(messages), std::back_inserter(message_ids), 
      [](const std::shared_ptr<Message>& msg)
      {
        return msg->id();
      });

    return remove_messages(message_ids);
  }

  bool Channel::edit_permissions(std::shared_ptr<Overwrite> overwrite, 
    std::function<void(std::shared_ptr<Permission>, std::shared_ptr<Permission>)> callback) const
  {
    callback(overwrite->allow(), overwrite->deny());
    return Discord::API::Channel::edit_permissions(m_id, overwrite,
              overwrite->allow()->get(), overwrite->deny()->get(), overwrite->type());
  }

  bool Channel::edit_permissions(std::shared_ptr<Overwrite> overwrite, Permission allow, Permission deny) const
  {
    return Discord::API::Channel::edit_permissions(m_id, overwrite, allow.get(), deny.get(), overwrite->type());
  }

  std::vector<std::shared_ptr<Invite>> Channel::get_invites() const
  {
    return Discord::API::Channel::get_invites(m_id);
  }

  std::shared_ptr<Invite> Channel::create_invite(uint32_t max_age, uint32_t max_uses, bool temporary, bool unique) const
  {
    return Discord::API::Channel::create_invite(m_id, max_age, max_uses, temporary, unique);
  }

  bool Channel::delete_permission(std::shared_ptr<Overwrite> overwrite) const
  {
    return Discord::API::Channel::remove_permission(m_id, overwrite);
  }

  bool Channel::start_typing() const
  {
    return Discord::API::Channel::trigger_typing_indicator(m_id);
  }

  std::vector<std::shared_ptr<Message>> Channel::get_pinned() const
  {
    return Discord::API::Channel::get_pinned_messages(m_id);
  }

  bool Channel::add_pin(Snowflake message_id) const
  {
    return Discord::API::Channel::add_pinned_message(m_id, message_id);
  }

  bool Channel::remove_pin(Snowflake message_id) const
  {
    return Discord::API::Channel::remove_pinned_message(m_id, message_id);
  }
}
