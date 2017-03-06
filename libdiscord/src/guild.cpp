#include "guild.h"

#include "api/api_channel.h"
#include "api/api_guild.h"
#include "channel.h"
#include "emoji.h"
#include "events.h"
#include "member.h"
#include "role.h"
#include "user.h"
#include "voice.h"

namespace Discord
{
  Guild::Guild()
  {
    m_afk_timeout = 0;
    m_embed_enabled = false;
    m_verification_level = VerificationLevel::None;
    m_default_message_notifications = NotificationLevel::None;
    m_mfa_level = 0;
    m_large = false;
    m_member_count = 0;
    m_unavailable = false;
  }

  Guild::Guild(const nlohmann::json& data)
  {
    set_id_from_json("id", data);
    set_from_json(m_name, "name", data);
    set_from_json(m_icon, "icon", data);
    set_from_json(m_splash, "splash", data);
    set_from_json(m_owner_id, "owner_id", data);
    set_from_json(m_region, "region", data);
    set_from_json(m_afk_channel_id, "afk_channel_id", data);
    set_from_json(m_afk_timeout, "afk_timeout", data);
    set_from_json(m_embed_enabled, "embed_enabled", data);
    set_from_json(m_embed_channel_id, "embed_channel_id", data);
    set_from_json(m_verification_level, "verification_level", data);
    set_from_json(m_default_message_notifications, "default_message_notifications", data);
    set_from_json(m_roles, "roles", data);
    set_from_json(m_emojis, "emojis", data);
    set_from_json(m_features, "features", data);
    set_from_json(m_mfa_level, "mfa_level", data);
    set_from_json(m_joined_at, "joined_at", data);
    set_from_json(m_large, "large", data);
    set_from_json(m_member_count, "member_count", data);
    set_from_json(m_voice_states, "voice_states", data);
    set_from_json(m_channels, "channels", data);
    set_from_json(m_unavailable, "unavailable", data);

    //  Add each channel in this guild to the cache
    for (auto& channel : m_channels)
    {
      //  Each channel should know what guild it is in.
      channel->set_guild_id(m_id);
      Discord::API::Channel::update_cache(channel);
    }

    if (data.count("members"))
    {
      auto members = data["members"].get<std::vector<std::shared_ptr<Member>>>();
      
      for (auto& member : members)
      {
        m_members[member->user()->id()] = member;
      }
    }

    if (data.count("presences"))
    {
      auto presences = data["presences"].get<std::vector<std::shared_ptr<PresenceUpdate>>>();

      for (auto& presence : presences)
      {
        m_presences[presence->user()->id()] = presence;
      }
    }
  }

  void Guild::merge(std::shared_ptr<Guild> other)
  {
    m_name = other->m_name;
    m_icon = other->m_icon;
    m_splash = other->m_splash;
    m_owner_id = other->m_owner_id;
    m_region = other->m_region;
    m_afk_channel_id = other->m_afk_channel_id;
    m_afk_timeout = other->m_afk_timeout;
    m_embed_enabled = other->m_embed_enabled;
    m_embed_channel_id = other->m_embed_channel_id;
    m_verification_level = other->m_verification_level;
    m_default_message_notifications = other->m_default_message_notifications;
    m_roles = other->m_roles;
    m_emojis = other->m_emojis;
    m_features = other->m_features;
    m_mfa_level = other->m_mfa_level;
    m_joined_at = other->m_joined_at;
    m_large = other->m_large;
    m_member_count = other->m_member_count;
    m_voice_states = other->m_voice_states;
    m_members = other->m_members;
    m_channels = other->m_channels;
    m_presences = other->m_presences;

    m_unavailable = other->m_unavailable;
  }

  std::string Guild::name() const
  {
    return m_name;
  }

  std::string Guild::region() const
  {
    return m_region;
  }

  VerificationLevel Guild::verification_level() const
  {
    return m_verification_level;
  }

  NotificationLevel Guild::notification_level() const
  {
    return m_default_message_notifications;
  }

  Snowflake Guild::afk_channel() const
  {
    return m_afk_channel_id;
  }

  uint32_t Guild::afk_timeout() const
  {
    return m_afk_timeout;
  }

  Snowflake Guild::owner_id() const
  {
    return m_owner_id;
  }

  std::shared_ptr<User> Guild::owner() const
  {
    return get_user(m_owner_id);
  }

  std::vector<std::shared_ptr<Emoji>> Guild::emojis() const
  {
    return m_emojis;
  }

  uint32_t Guild::member_count() const
  {
    return m_member_count;
  }

  std::shared_ptr<User> Guild::get_user(Snowflake user_id) const
  {
    auto user_itr = m_members.find(user_id);

    if (user_itr == std::end(m_members))
    {
      LOG(ERROR) << "Could not find user with id " << user_id.to_string();
      return std::make_shared<User>();
    }

    return user_itr->second->user();
  }

  void Guild::set_name(std::string name)
  {
    m_name = name;
  }

  void Guild::set_region(std::string region)
  {
    m_region = region;
  }

  void Guild::set_verification_level(VerificationLevel level)
  {
    m_verification_level = level;
  }

  void Guild::set_notification_level(NotificationLevel level)
  {
    m_default_message_notifications = level;
  }

  void Guild::set_afk_channel(Snowflake channel_id)
  {
    m_afk_channel_id = channel_id;
  }

  void Guild::set_afk_timeout(uint32_t timeout)
  {
    m_afk_timeout = timeout;
  }

  void Guild::set_owner(Snowflake user_id)
  {
    m_owner_id = user_id;
  }

  void Guild::set_emojis(std::vector<std::shared_ptr<Emoji>> emojis)
  {
    m_emojis = emojis;
  }

  void Guild::set_unavailable(bool value)
  {
    m_unavailable = true;
  }

  void Guild::add_member(std::shared_ptr<Member> member)
  {
    if (m_members.count(member->user()->id()))
    {
      LOG(ERROR) << "Tried to add a user that already exists. Ignoring.";
      return;
    }

    m_members[member->user()->id()] = member;
    m_member_count += 1;
  }

  void Guild::remove_member(std::shared_ptr<Member> member)
  {
    if (m_members.count(member->user()->id()))
    {
      m_members.erase(member->user()->id());
      m_member_count -= 1;
    }
  }

  void Guild::update_member(std::vector<Snowflake> roles, std::shared_ptr<User> user, std::string nick)
  {
    std::shared_ptr<Member> member;
    auto member_itr = m_members.find(user->id());

    if (member_itr == std::end(m_members))
    {
      //  Previously unseen user being updated, so make a new one.
      member = std::make_shared<Member>();
    }
    else
    {
      member = member_itr->second;
    }

    member->set_roles(roles);
    member->set_user(user);
    member->set_nick(nick);
  }

  void Guild::add_role(Role role)
  {
    m_roles.push_back(std::make_shared<Role>(role));
  }

  void Guild::remove_role(Snowflake id)
  {
    m_roles.erase(
      std::remove_if(std::begin(m_roles), std::end(m_roles),
        [id](std::shared_ptr<Role> old) { return old->id() == id; }));
  }

  void Guild::update_role(Role role)
  {
    auto old_role = std::find_if(std::begin(m_roles), std::end(m_roles), [role](std::shared_ptr<Role> old) { return old->id() == role.id(); });

    if (old_role == std::end(m_roles))
    {
      LOG(ERROR) << "Update Role was called with previously unseen role. Ignoring.";
      return;
    }

    auto old = *old_role;
    old->merge(role);
  }

  void Guild::add_channel(std::shared_ptr<Channel> channel)
  {
    m_channels.push_back(channel);
  }

  void Guild::remove_channel(std::shared_ptr<Channel> channel)
  {
    m_channels.erase(std::remove_if(std::begin(m_channels), std::end(m_channels), [channel](std::shared_ptr<Channel> chan)
    {
      return chan->id() == channel->id();
    }));
  }

  void Guild::update_presence(std::shared_ptr<PresenceUpdate> presence)
  {
    if (m_presences.count(presence->user()->id()))
    {
      m_presences[presence->user()->id()]->merge(presence);
    }
    else
    {
      m_presences[presence->user()->id()] = presence;
    }
  }

  std::shared_ptr<Channel> Guild::find_channel(std::string name)
  {
    auto found = std::find_if(std::begin(m_channels), std::end(m_channels), [name](std::shared_ptr<Channel> channel)
    {
      return channel->name() == name;
    });

    if (found == std::end(m_channels))
    {
      return nullptr;
    }

    return *found;
  }

  std::shared_ptr<Guild> Guild::modify(std::function<void(std::shared_ptr<Guild>)> modify_block) const
  {
    auto guild = Discord::API::Guild::get(m_id);

    modify_block(guild);

    return Discord::API::Guild::modify(m_id, guild);
  }

  std::shared_ptr<Guild> Guild::remove() const
  {
    return Discord::API::Guild::remove(m_id);
  }

  std::vector<std::shared_ptr<Channel>> Guild::channels() const
  {
    return Discord::API::Guild::get_channels(m_id);
  }

  std::shared_ptr<Channel> Guild::create_text_channel(std::string name, std::vector<std::shared_ptr<Overwrite>> permission_overwrites) const
  {
    return Discord::API::Guild::create_text_channel(m_id, name, permission_overwrites);
  }

  std::shared_ptr<Channel> Guild::create_voice_channel(std::string name, uint32_t bitrate, uint32_t user_limit, std::vector<std::shared_ptr<Overwrite>> permission_overwrites) const
  {
    return Discord::API::Guild::create_voice_channel(m_id, name, bitrate, user_limit, permission_overwrites);
  }

  std::vector<std::shared_ptr<Channel>> Guild::reorder_channels(const std::map<Snowflake, uint32_t>& positions) const
  {
    return Discord::API::Guild::modify_channel_positions(m_id, positions);
  }

  UserGuild::UserGuild()
  {
    m_owner = false;
  }

  UserGuild::UserGuild(const nlohmann::json& data)
  {
    set_id_from_json("id", data);
    set_from_json(m_name, "name", data);
    set_from_json(m_icon, "icon", data);
    set_from_json(m_owner, "owner", data);
    set_from_json(m_permissions, "permissions", data);
  }

  std::string UserGuild::name() const
  {
    return m_name;
  }

  std::string UserGuild::icon() const
  {
    return m_icon;
  }

  bool UserGuild::owner() const
  {
    return m_owner;
  }

  std::shared_ptr<Permission> UserGuild::permissions() const
  {
    return m_permissions;
  }

  GuildEmbed::GuildEmbed()
  {
    m_enabled = false;
  }

  GuildEmbed::GuildEmbed(const nlohmann::json& data)
  {
    set_from_json(m_enabled, "enabled", data);
    set_from_json(m_channel_id, "channel_id", data);
  }

  bool GuildEmbed::enabled() const
  {
    return m_enabled;
  }

  Snowflake GuildEmbed::channel_id() const
  {
    return m_channel_id;
  }
}
