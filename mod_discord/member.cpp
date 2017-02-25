#include "member.h"

namespace ModDiscord
{
  Member::Member()
  {
    m_deaf = false;
    m_mute = false;
  }

  Member::Member(const nlohmann::json& data)
  {
    set_from_json(m_user, "user", data);
    set_from_json(m_nick, "nick", data);
    set_from_json(m_roles, "roles", data);
    set_from_json(m_joined_at, "joined_at", data);
    set_from_json(m_deaf, "deaf", data);
    set_from_json(m_mute, "mute", data);
  }
}