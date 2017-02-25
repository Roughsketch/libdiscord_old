#include "user.h"
#include "external/json.hpp"

namespace ModDiscord
{
  User::User()
  {
    m_bot = false;
    m_mfa_enabled = false;
    m_verified = false;
  }

  User::User(nlohmann::json data)
  {
    set_id_from_json("id", data);
    set_from_json(m_username, "username", data);
    set_from_json(m_discriminator, "discriminator", data);
    set_from_json(m_avatar, "avatar", data);
    set_from_json(m_bot, "bot", data);
    set_from_json(m_mfa_enabled, "mfa_enabled", data);
    set_from_json(m_verified, "verified", data);
    set_from_json(m_email, "email", data);
  }

  std::string User::username() const
  {
    return m_username;
  }

  std::string User::discriminator() const
  {
    return m_discriminator;
  }

  std::string User::distinct() const
  {
    return username() + "#" + discriminator();
  }

  std::string User::email() const
  {
    return m_email;
  }

  std::string User::avatar_id() const
  {
    return m_avatar;
  }
}
