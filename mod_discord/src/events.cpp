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
    m_user = std::make_shared<User>(data);
    set_from_json(m_roles, "roles", data);
    set_from_json(m_game, "game", data);
    set_from_json(m_guild_id, "guild_id", data);
    set_from_json(m_status, "status", data);
  }
}
