#include "permission.h"

namespace Discord
{
  Permission::Permission()
  {
    m_permissions = 0;
  }

  Permission::Permission(const nlohmann::json& data)
  {
    m_permissions = data.get<uint32_t>();
  }

  uint32_t Permission::get() const
  {
    return m_permissions;
  }

  void Permission::add(Permissions permission)
  {
    m_permissions |= permission;
  }

  void Permission::remove(Permissions permission)
  {
    m_permissions &= ~permission;
  }
}
