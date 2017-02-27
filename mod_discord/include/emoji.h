#pragma once

#include "external/json.hpp"
#include "identifiable.h"
#include "role.h"

namespace ModDiscord
{
  class Emoji : public Identifiable
  {
    std::string m_name;
    std::vector<Snowflake> m_roles;
    bool m_require_colons;
    bool m_managed;
  public:
    Emoji();
    explicit Emoji(const nlohmann::json& data);

    std::string name() const;
    std::vector<Snowflake> roles() const;
  };

  inline void from_json(const nlohmann::json& json, Emoji& emoji)
  {
    emoji = Emoji(json);
  }
}
