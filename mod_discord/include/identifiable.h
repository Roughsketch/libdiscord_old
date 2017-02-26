#pragma once

#include "common.h"

namespace ModDiscord
{
  class Identifiable
  {
  protected:
    snowflake m_id;
  public:
    Identifiable() : m_id(0) {}

    explicit operator snowflake() const
    {
      return m_id;
    }

    template <typename T>
    void set_id_from_json(T key, nlohmann::json data)
    {
      set_from_json(m_id, key, data);
    }

    void set_id(snowflake id)
    {
      m_id = id;
    }

    snowflake id() const
    {
      return m_id;
    }
  };
}