#include "guild.h"

namespace ModDiscord
{
  Guild::Guild(nlohmann::json data)
  {
    set_id_from_json("id", data);

    for (auto& channel : data["channels"])
    {
      //  Set the guild_id member for each channel since it is missing.
      channel["guild_id"] = std::to_string(m_id);
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
}