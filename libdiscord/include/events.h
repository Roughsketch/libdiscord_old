#pragma once

#include "common.h"

namespace Discord
{
  class User;

  /** Enumeration of types that a Game object can be.
      Current only valid options are Stream and Normal. */
  enum GameType : uint8_t
  {
    Normal = 0,
    Stream
  };

  class Game
  {
    std::string m_name;
    GameType m_type;
    std::string m_url;
  public:
    Game();
    explicit Game(const nlohmann::json& data);

    /** Get the name of the game.
     
        @return The game's name.
     */
    std::string name() const;

    /** Get the type of this game status.

        @return The type of this game status.
     */
    GameType type() const;

    /** Gets the streaming URL if available.
     
        @return The streaming URL, or empty if not streaming.
     */
    std::string url() const;
  };

  inline void from_json(const nlohmann::json& json, Game& game)
  {
    game = Game(json);
  }

  /** Represents a presence update event. */
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

    /** Overwrite data in this object with another.
     
        @param other The presence object to merge into this one.
     */
    void merge(std::shared_ptr<PresenceUpdate> other);

    /** Get the user that this presence is about.
        
        @return A shared pointer to the user.
      */
    std::shared_ptr<User> user() const;
  };

  inline void from_json(const nlohmann::json& json, PresenceUpdate& presence)
  {
    presence = PresenceUpdate(json);
  }
}
