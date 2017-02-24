#pragma once

#include <memory>

#include "common.h"
#include "gateway.h"

namespace ModDiscord
{
  class Bot
  {
    std::string m_token;
    snowflake m_client_id;

    std::shared_ptr<Gateway> m_gateway;
  public:
    explicit Bot(nm::json);

    std::string invite_url() const;
  };
}
