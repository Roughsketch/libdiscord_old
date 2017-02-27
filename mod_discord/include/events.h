#pragma once
#include "user.h"

namespace ModDiscord
{
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
    User m_user;
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
  /*
  void to_json(nlohmann::json& data, const Presense& p)
  {
    data = {
      {"user", p.user},
      {"roles", nlohmann::json::array({p.roles})},
      {"game", p.game},
      {"guild_id", p.guild_id},
      {"status", p.status}
    };
  }

  void from_json(const nlohmann::json& data, Presense& p)
  {
    p.user = data["user"].get<User>();
  }
  */
}
