#include "voice.h"

namespace Discord
{
  VoiceState::VoiceState()
  {
    m_deaf = false;
    m_mute = false;
    m_self_deaf = false;
    m_self_mute = false;
    m_suppress = false;
  }

  VoiceState::VoiceState(const nlohmann::json& data)
  {
    set_from_json(m_guild_id, "guild_id", data);
    set_from_json(m_channel_id, "channel_id", data);
    set_from_json(m_user_id, "user_id", data);
    set_from_json(m_session_id, "session_id", data);
    set_from_json(m_deaf, "deaf", data);
    set_from_json(m_mute, "mute", data);
    set_from_json(m_self_deaf, "self_deaf", data);
    set_from_json(m_self_mute, "self_mute", data);
    set_from_json(m_suppress, "suppress", data);
  }
}