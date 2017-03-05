#include "emoji.h"

namespace Discord
{
  Emoji::Emoji()
  {
    m_require_colons = false;
    m_managed = false;
  }

  Emoji::Emoji(const nlohmann::json& data)
  {
    set_id_from_json("id", data);
    set_from_json(m_name, "name", data);
    set_from_json(m_roles, "roles", data);
    set_from_json(m_require_colons, "require_colons", data);
    set_from_json(m_managed, "managed", data);
  }

  std::string Emoji::name() const
  {
    return m_name;
  }

  std::vector<Snowflake> Emoji::roles() const
  {
    return m_roles;
  }

  Reaction::Reaction()
  {
    m_count = 0;
    m_me = false;
  }

  Reaction::Reaction(const nlohmann::json& data)
  {
    set_from_json(m_count, "count", data);
    set_from_json(m_me, "me", data);
    set_from_json(m_emoji, "emoji", data);
  }

  uint32_t Reaction::count() const
  {
    return m_count;
  }

  bool Reaction::me() const
  {
    return m_me;
  }

  std::string Reaction::name() const
  {
    return m_emoji->name();
  }

  Snowflake Reaction::id() const
  {
    return m_emoji->id();
  }
}
