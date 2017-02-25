#pragma once

#include "common.h"

namespace ModDiscord
{
  class VoiceState
  {
    snowflake m_guild_id;
    snowflake m_channel_id;
    snowflake m_user_id;
    std::string m_session_id;
    bool m_deaf;
    bool m_mute;
    bool m_self_deaf;
    bool m_self_mute;
    bool m_suppress;
  public:
    VoiceState();
    explicit VoiceState(const nlohmann::json& data);
  };

  inline void from_json(const nlohmann::json& json, VoiceState& state)
  {
    state = VoiceState(json);
  }
}