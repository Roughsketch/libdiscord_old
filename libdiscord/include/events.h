#pragma once

#include "common.h"

namespace ModDiscord
{
  class User;

  class Game
  {
    std::string m_name;
    uint32_t m_type;
    std::string m_url;
  public:
    Game();
    explicit Game(const nlohmann::json& data);
  };

  inline void from_json(const nlohmann::json& json, Game& game)
  {
    game = Game(json);
  }

  class PresenceUpdate
  {
    std::shared_ptr<User> m_user;
    std::vector<Snowflake> m_roles;
    Game m_game;
    Snowflake m_guild_id;
    std::string m_status;
  public:
    PresenceUpdate();
    explicit PresenceUpdate(const nlohmann::json& data);
  };

  inline void from_json(const nlohmann::json& json, PresenceUpdate& presence)
  {
    presence = PresenceUpdate(json);
  }
}