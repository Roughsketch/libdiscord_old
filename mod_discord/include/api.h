#pragma once

#include <future>

#include <cpprest/http_client.h>
#include "external/json.hpp"

#include "api/api_channel.h"
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

    static utility::string_t Token;

    void set_token(std::string token);
    std::string get_wss_url();
    nlohmann::json request(RequestType type, std::string endpoint, nlohmann::json data = {});
  }
}
