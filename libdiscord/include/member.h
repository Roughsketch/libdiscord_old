#pragma once

#include <chrono>
#include <vector>

#include "common.h"

namespace Discord
{
  class User;

  class Member
  {
    std::shared_ptr<User> m_user;
    std::string m_nick;
    std::vector<Snowflake> m_roles;
    std::string m_joined_at;
    bool m_deaf;
    bool m_mute;
  public:
    Member();
    explicit Member(const nlohmann::json& data);

    std::shared_ptr<User> user() const;
    std::vector<Snowflake> roles() const;
    std::string nick() const;
    std::string nickname() const;

    void set_user(std::shared_ptr<User> user);
    void set_nick(std::string nick);
    void set_roles(std::vector<Snowflake> role_ids);
  };

  inline void from_json(const nlohmann::json& json, Member& member)
  {
    member = Member(json);
  }
}
