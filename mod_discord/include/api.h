#pragma once

#include "external/json.hpp"

#include "api/api_channel.h"
#include "api/api_guild.h"
#include "common.h"

namespace ModDiscord
{
  namespace API
  {
    enum RequestType : uint8_t
    {
      GET,
      POST,
      PUT,
      DELETE,
      PATCH
    };

    enum Status : uint16_t
    {
      OK = 200,
      NoContent = 204
    };

    void set_token(std::string token);
    std::string get_wss_url();
    nlohmann::json request(RequestType type, std::string endpoint, nlohmann::json data = {});
  }
}
