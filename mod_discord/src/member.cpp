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

  User Member::user() const
  {
    return m_user;
  }

  std::vector<Snowflake> Member::roles() const
  {
    return m_roles;
  }

  std::string Member::nick() const
  {
    return m_nick;
  }

  std::string Member::nickname() const
  {
    return nick();
  }

  void Member::set_user(User user)
  {
    m_user = user;
  }

  void Member::set_nick(std::string nick)
  {
    m_nick = nick;
  }

  void Member::set_roles(std::vector<Snowflake> role_ids)
  {
    m_roles = role_ids;
  }
}
