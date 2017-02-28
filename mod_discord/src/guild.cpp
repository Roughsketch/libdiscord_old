#include "guild.h"

#include "channel.h"
#include "emoji.h"
#include "events.h"
#include "member.h"
#include "role.h"
#include "user.h"
#include "voice.h"

namespace ModDiscord
{
  Guild::Guild()
  {
    m_afk_timeout = 0;
    m_embed_enabled = false;
    m_verification_level = 0;
    m_default_message_notifications = 0;
    m_mfa_level = 0;
    m_large = false;
    m_member_count = 0;
    m_unavailable = false;
  }

  Guild::Guild(nlohmann::json data)
  {
    set_id_from_json("id", data);

    for (auto& channel : data["channels"])
    {
      //  Set the guild_id member for each channel since it is missing.
      channel["guild_id"] = m_id;
    }

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
    set_from_json(m_members, "members", data);
    set_from_json(m_channels, "channels", data);
    set_from_json(m_presences, "presences", data);
    set_from_json(m_unavailable, "unavailable", data);
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

  std::vector<std::shared_ptr<Emoji>> Guild::emojis() const
  {
    return m_emojis;
  }

  void Guild::set_emojis(std::vector<std::shared_ptr<Emoji>> emojis)
  {
    m_emojis = emojis;
  }

  void Guild::set_unavailable(bool value)
  {
    m_unavailable = true;
  }

  void Guild::add_member(Member member)
  {
    m_members.push_back(std::make_shared<Member>(member));
    m_member_count += 1;
  }

  void Guild::remove_member(Member member)
  {
    m_members.erase(std::remove_if(std::begin(m_members), std::end(m_members), 
      [member](std::shared_ptr<Member> other) {
        return member.user()->id() == other->user()->id();
      })
    );

    m_member_count -= 1;
  }

  void Guild::update_member(std::vector<Snowflake> roles, User user, std::string nick)
  {
    auto member_itr = std::find_if(std::begin(m_members), std::end(m_members), 
      [user](std::shared_ptr<Member> m) {
        return m->user()->id() == user.id();
      }
    );

    if (member_itr == std::end(m_members))
    {
      LOG(ERROR) << "Update member called with previously unseen member. Ignoring.";
      return;
    }

    auto member = *member_itr;

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
}
