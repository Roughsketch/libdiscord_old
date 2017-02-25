#include "role.h"

namespace ModDiscord
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
    set_from_json(m_id, "id", data);
    set_from_json(m_name, "name", data);
    set_from_json(m_color, "color", data);
    set_from_json(m_hoist, "hoist", data);
    set_from_json(m_position, "position", data);
    set_from_json(m_permissions, "permissions", data);
    set_from_json(m_managed, "managed", data);
    set_from_json(m_mentionable, "mentionable", data);
  }
}
