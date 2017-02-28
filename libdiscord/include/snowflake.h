#pragma once

#include <cstdint>
#include <string>

#include "external/json.hpp"

class Snowflake
{
  uint64_t m_id;
public:
  Snowflake() : m_id(0) {};
  explicit Snowflake(uint64_t id) : m_id(id) {};

  explicit Snowflake(const std::string s)
  {
    m_id = std::stoull(s);
  }

  bool operator==(const Snowflake& rhs) const
  {
    return m_id == rhs.m_id;
  }

  bool operator<(const Snowflake& rhs) const
  {
    return m_id < rhs.m_id;
  }

  explicit operator std::string() const
  {
    return std::to_string(m_id);
  }

  explicit operator uint64_t() const
  {
    return m_id;
  }

  uint64_t operator()(Snowflake& s) const
  {
    return m_id;
  }

  std::string to_string() const
  {
    return std::to_string(m_id);
  }
};

inline void to_json(nlohmann::json& json, const Snowflake& id)
{
  json = id.to_string();
}

inline void from_json(const nlohmann::json& json, Snowflake& id)
{
  id = Snowflake(json.get<std::string>());
}
