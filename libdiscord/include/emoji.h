#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class Emoji : public Identifiable
  {
    std::string m_name;
    std::vector<Snowflake> m_roles;
    bool m_require_colons;
    bool m_managed;
  public:
    Emoji();
    explicit Emoji(nlohmann::json data);

    /** Get the name of this emoji.
     
        @returns The name for this emoji.
     */
    std::string name() const;

    /** Get a vector of role ids that can use this emoji.

        @returns A vector of role ids that can use this emoji.
    */
    std::vector<Snowflake> roles() const;
  };

  inline void from_json(const nlohmann::json& json, Emoji& emoji)
  {
    emoji = Emoji(json);
  }

  class Reaction
  {
    uint32_t m_count;
    bool m_me;
    std::shared_ptr<Emoji> m_emoji;
  public:
    Reaction();
    explicit Reaction(nlohmann::json data);

    uint32_t count() const;
    bool me() const;
    std::string name() const;
    Snowflake id() const;
  };

  inline void from_json(const nlohmann::json& json, Reaction& reaction)
  {
    reaction = Reaction(json);
  }
}
