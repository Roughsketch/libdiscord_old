#include "role.h"

namespace Discord
{
  Role::Role()
  {
    m_color = 0;
    m_hoist = false;
    m_position = 0;
    m_permissions = 0;
    m_managed = false;
    m_mentionable = false;
  }

  Role::Role(const nlohmann::json& data)
  {
    set_id_from_json("id", data);
    set_from_json(m_name, "name", data);
    set_from_json(m_color, "color", data);
    set_from_json(m_hoist, "hoist", data);
    set_from_json(m_position, "position", data);
    set_from_json(m_permissions, "permissions", data);
    set_from_json(m_managed, "managed", data);
    set_from_json(m_mentionable, "mentionable", data);
  }

  void Role::merge(Role other)
  {
    m_name = other.m_name;
    m_color = other.m_color;
    m_hoist = other.m_hoist;
    m_position = other.m_position;
    m_permissions = other.m_permissions;
    m_managed = other.m_managed;
    m_mentionable = other.m_mentionable;
  }
}
