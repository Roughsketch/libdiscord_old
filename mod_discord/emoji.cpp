#include "emoji.h"

namespace ModDiscord
{
  Emoji::Emoji()
  {
    m_require_colons = false;
    m_managed = false;
  }

  Emoji::Emoji(const nlohmann::json& data)
  {
    set_from_json(m_id, "id", data);
    set_from_json(m_name, "name", data);
    set_from_json(m_roles, "roles", data);
    set_from_json(m_require_colons, "require_colons", data);
    set_from_json(m_managed, "managed", data);
  }
}
