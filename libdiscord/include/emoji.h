#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  /** Represents an Emoji object. */
  class Emoji : public Identifiable
  {
    std::string m_name;
    std::vector<Snowflake> m_roles;
    bool m_require_colons;
    bool m_managed;
  public:
    Emoji();
    explicit Emoji(const nlohmann::json& data);

    /** Get the name of this emoji.
     
        @return The name for this emoji.
     */
    std::string name() const;

    /** Get a vector of role ids that can use this emoji.

        @return A vector of role ids that can use this emoji.
    */
    std::vector<Snowflake> roles() const;

    /** Get a string that will be turned into this emoji when sent.
     
        @return The mentionable string for this emoji.
     */
    std::string mention() const;
  };

  inline void from_json(const nlohmann::json& json, Emoji& emoji)
  {
    emoji = Emoji(json);
  }

  /** Represents a reaction to a message. */
  class Reaction
  {
    uint32_t m_count;
    bool m_me;
    std::shared_ptr<Emoji> m_emoji;
  public:
    Reaction();
    explicit Reaction(const nlohmann::json& data);

    /** How many times this reaction has been used.
     
        @return How many uses this reaction has gotten.
     */
    uint32_t count() const;

    /** Whether or not the current user has reacted with this reaction yet.
     
        @return Whether this user has already reacted.
     */
    bool me() const;

    /** Get the name of the emoji used in this reaction.
     
        @return The name of the emoji used in this reaction.
     */
    std::string name() const;

    /** Get the id of the emoji used in this reaction.
     
        @return The id of the emoji used in this reaction.
     */
    Snowflake id() const;
  };

  inline void from_json(const nlohmann::json& json, Reaction& reaction)
  {
    reaction = Reaction(json);
  }
}
