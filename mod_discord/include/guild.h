#pragma once

#include <chrono>
#include <vector>

#include "common.h"
#include "channel.h"
#include "emoji.h"
#include "events.h"
#include "identifiable.h"
#include "member.h"
#include "role.h"
#include "voice.h"

namespace ModDiscord
{
  class Guild : public Identifiable
  {
    std::string m_name;
    std::string m_icon;
    std::string m_splash;
    Snowflake m_owner_id;
    std::string m_region;
    Snowflake m_afk_channel_id;
    uint32_t m_afk_timeout;
    bool m_embed_enabled;
    Snowflake m_embed_channel_id;
    uint32_t m_verification_level;
    uint32_t m_default_message_notifications;
    std::vector<Role> m_roles;
    std::vector<Emoji> m_emojis;
    std::vector<std::string> m_features;
    uint32_t m_mfa_level;
    std::string m_joined_at;
    bool m_large;
    uint32_t m_member_count;
    std::vector<VoiceState> m_voice_states;
    std::vector<Member> m_members;
    std::vector<Channel> m_channels;
    std::vector<PresenceUpdate> m_presences;

    bool m_unavailable;
  public:
    Guild();
    explicit Guild(nlohmann::json data);

    void merge(std::shared_ptr<Guild> other);

    std::string name() const;
    std::vector<Emoji> emojis() const;

    void set_emojis(std::vector<Emoji> emojis);
    void set_unavailable(bool value);

    void add_member(Member member);
    void remove_member(Member member);
    void update_member(std::vector<Snowflake> roles, User user, std::string nick);
  };

  inline void from_json(const nlohmann::json& json, Guild& guild)
  {
    guild = Guild(json);
  }
}
