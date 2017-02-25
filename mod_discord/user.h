#pragma once

#include "common.h"

namespace ModDiscord
{
  class User : public Identifiable
  {
    std::string m_username;
    std::string m_discriminator;
    std::string m_avatar;
    bool m_bot;
    bool m_mfa_enabled;
    bool m_verified;
    std::string m_email;
  public:
    User();
    explicit User(nlohmann::json data);

    std::string username() const;
    std::string discriminator() const;
    std::string distinct() const;
    std::string email() const;
    std::string avatar_id() const;
  };

  inline void from_json(const nlohmann::json& json, User& user)
  {
    user = User(json);
  }
}