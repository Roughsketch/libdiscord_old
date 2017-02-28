#pragma once

#include "common.h"

namespace ModDiscord
{
  class Identifiable
  {
  protected:
    Snowflake m_id;
  public:
    Identifiable() : m_id(0) {}
    explicit Identifiable(Snowflake id) : m_id(id) {}

    bool operator<(const Identifiable& rhs) const
    {
      return m_id < rhs.m_id;
    }

    explicit operator Snowflake() const
    {
      return m_id;
    }

    template <typename T>
    void set_id_from_json(T key, nlohmann::json data)
    {
      set_from_json(m_id, key, data);
    }

    void set_id(Snowflake id)
    {
      m_id = id;
    }

    Snowflake id() const
    {
      return m_id;
    }
  };
}