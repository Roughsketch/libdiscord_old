#pragma once
#include <cstdint>
#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class Role : public Identifiable
  {
    std::string m_name;
    uint32_t m_color;
    bool m_hoist;
    uint32_t m_position;
    uint32_t m_permissions;
    bool m_managed;
    bool m_mentionable;
  public:
    Role();
    explicit Role(nlohmann::json data);

    void merge(Role other);
  };

  inline void from_json(const nlohmann::json& json, Role& role)
  {
    role = Role(json);
  }
}
