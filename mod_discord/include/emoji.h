#pragma once

#include "external/json.hpp"
#include "identifiable.h"
#include "role.h"

namespace ModDiscord
{
  class Emoji : public Identifiable
  {
    std::string m_name;
    std::vector<snowflake> m_roles;
    bool m_require_colons;
    bool m_managed;
  public:
    Emoji();
    explicit Emoji(const nlohmann::json& data);
  };

  inline void from_json(const nlohmann::json& json, Emoji& emoji)
  {
    emoji = Emoji(json);
  }
}
