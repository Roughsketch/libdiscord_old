#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class User;

  class IntegrationAccount
  {
    std::string m_id;
    std::string m_name;
  public:
    explicit IntegrationAccount(const nlohmann::json& data);

    /** Get the id of this integration account.
     
        @return The id of this account.
     */
    std::string id() const;

    /** Get the name of this integration account.
     
        @return The name of this account.
     */
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
    explicit Integration(const nlohmann::json& data);
  };
}