#pragma once

#include <chrono>
#include <vector>

#include "common.h"
#include "role.h"
#include "user.h"

namespace ModDiscord
{
  class Member
  {
    User m_user;
    std::string m_nick;
    std::vector<Snowflake> m_roles;
    std::string m_joined_at;
    bool m_deaf;
    bool m_mute;
  public:
    Member();
    explicit Member(const nlohmann::json& data);

    User user() const;
    std::vector<Snowflake> roles() const;
    std::string nick() const;
    std::string nickname() const;

    void set_user(User user);
    void set_nick(std::string nick);
    void set_roles(std::vector<Snowflake> role_ids);
  };

  inline void from_json(const nlohmann::json& json, Member& member)
  {
    member = Member(json);
  }
}
