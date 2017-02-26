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
    std::vector<snowflake> m_roles;
    std::string m_joined_at;
    bool m_deaf;
    bool m_mute;
  public:
    Member();
    explicit Member(const nlohmann::json& data);
  };

  inline void from_json(const nlohmann::json& json, Member& member)
  {
    member = Member(json);
  }
}
