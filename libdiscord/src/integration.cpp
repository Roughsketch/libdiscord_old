#include "integration.h"
#include "user.h"

namespace Discord
{
  IntegrationAccount::IntegrationAccount(const nlohmann::json& data)
  {
    set_from_json(m_id, "id", data);
    set_from_json(m_name, "name", data);
  }

  std::string IntegrationAccount::id() const
  {
    return m_id;
  }

  std::string IntegrationAccount::name() const
  {
    return m_name;
  }

  Integration::Integration()
  {
    m_enabled = false;
    m_syncing = false;
    m_expire_behavior = 0;
    m_expire_grace_period = 0;
  }

  Integration::Integration(const nlohmann::json& data)
  {
    set_from_json(m_name, "name", data);
    set_from_json(m_type, "type", data);
    set_from_json(m_enabled, "enabled", data);
    set_from_json(m_syncing, "syncing", data);
    set_from_json(m_role_id, "role_id", data);
    set_from_json(m_expire_behavior, "expire_behavior", data);
    set_from_json(m_expire_grace_period, "expire_grace_period", data);
    set_from_json(m_user, "user", data);
    set_from_json(m_account, "account", data);
    set_from_json(m_timestamp, "timestamp", data);
  }
}
