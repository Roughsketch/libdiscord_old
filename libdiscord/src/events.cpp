#include "events.h"
#include "user.h"

namespace ModDiscord
{
  Game::Game() : m_type(0)
  {
  }

  Game::Game(const nlohmann::json& data)
  {
    set_from_json(m_name, "name", data);
    set_from_json(m_type, "type", data);
    set_from_json(m_url, "url", data);
  }

  PresenceUpdate::PresenceUpdate()
  {
  }

  PresenceUpdate::PresenceUpdate(const nlohmann::json& data)
  {
    set_from_json(m_user, "user", data);
    set_from_json(m_roles, "roles", data);
    set_from_json(m_game, "game", data);
    set_from_json(m_guild_id, "guild_id", data);
    set_from_json(m_status, "status", data);
  }

  void PresenceUpdate::merge(std::shared_ptr<PresenceUpdate> other)
  {
    m_user = other->m_user;
    m_roles = other->m_roles;
    m_game = other->m_game;
    m_guild_id = other->m_guild_id;
    m_status = other->m_status;
  }

  std::shared_ptr<User> PresenceUpdate::user() const
  {
    return m_user;
  }
}
