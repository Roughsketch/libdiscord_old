#include "guild.h"

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
    set_from_json(m_emoji, "emoji", data);
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
    m_emoji = other->m_emoji;
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

  void Guild::set_unavailable(bool value)
  {
    m_unavailable = true;
  }
}
