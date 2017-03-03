#include "integration.h"

namespace ModDiscord
{
  IntegrationAccount::IntegrationAccount(nlohmann::json data)
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
}
