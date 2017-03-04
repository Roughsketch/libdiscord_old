#pragma once
#include <cstdint>
#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class Permission;

  class Role : public Identifiable
  {
    std::string m_name;
    uint32_t m_color;
    bool m_hoist;
    uint32_t m_position;
    std::shared_ptr<Permission> m_permissions;
    bool m_managed;
    bool m_mentionable;
  public:
    Role();
    explicit Role(nlohmann::json data);

    void merge(Role other);

    std::string name() const;
    uint32_t color() const;
    bool hoisted() const;
    uint32_t position() const;
    std::shared_ptr<Permission> permissions() const;
    bool managed() const;
    bool mentionable() const;
  };

  inline void from_json(const nlohmann::json& json, Role& role)
  {
    role = Role(json);
  }

  inline void to_json(nlohmann::json& json, const Role& role)
  {
    json["name"] = role.name();
    json["color"] = role.color();
    json["hoist"] = role.hoisted();
    json["position"] = role.position();
    json["permissions"] = role.permissions();
    json["managed"] = role.managed();
    json["mentionable"] = role.mentionable();
  }
}
