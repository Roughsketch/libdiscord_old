#pragma once

#include "common.h"
#include "identifiable.h"

namespace ModDiscord
{
  class User;

  class IntegrationAccount
  {
    std::string m_id;
    std::string m_name;
  public:
    explicit IntegrationAccount(nlohmann::json data);

    std::string id() const;
    std::string name() const;
  };

  class Integration : public Identifiable
  {
    std::string m_name;
    std::string m_type;
    bool m_enabled;
    bool m_syncing;
    Snowflake m_role_id;
    uint32_t m_expire_behavior;
    uint32_t m_expire_grace_period;
    std::shared_ptr<User> m_user;
    std::shared_ptr<IntegrationAccount> m_account;
    std::string m_timestamp;
  public:
    Integration();
    explicit Integration(nlohmann::json data);
  };
}