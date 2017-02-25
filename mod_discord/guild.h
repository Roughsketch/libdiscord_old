#pragma once

#include <chrono>
#include <vector>

#include "common.h"
#include "channel.h"
#include "emoji.h"
#include "events.h"
#include "member.h"
#include "role.h"
#include "voice.h"

namespace ModDiscord
{
  class UnavailableGuild
  {
  protected:
    snowflake m_id;
    bool m_unavailable;
  public:
    UnavailableGuild();
    explicit UnavailableGuild(nlohmann::json data);
  };

  inline void from_json(const nlohmann::json& json, UnavailableGuild& guild)
  {
    guild = UnavailableGuild(json);
  }

  class Guild : private UnavailableGuild
  {
    std::string m_name;
    std::string m_icon;
    std::string m_splash;
    snowflake m_owner_id;
    std::string m_region;
    snowflake m_afk_channel_id;
    uint32_t m_afk_timeout;
    bool m_embed_enabled;
    snowflake m_embed_channel_id;
    uint32_t m_verification_level;
    uint32_t m_default_message_notifications;
    std::vector<Role> m_roles;
    std::vector<Emoji> m_emoji;
    std::vector<std::string> m_features;
    uint32_t m_mfa_level;
    std::string m_joined_at;
    bool m_large;
    uint32_t m_member_count;
    std::vector<VoiceState> m_voice_states;
    std::vector<Member> m_members;
    std::vector<Channel> m_channels;
    std::vector<PresenceUpdate> m_presences;
  public:
    explicit Guild(nlohmann::json data);

    bool operator< (const Guild& other) const
    {
      return m_id < other.m_id;
    }
  };

  inline void from_json(const nlohmann::json& json, Guild& guild)
  {
    guild = Guild(json);
  }
}
