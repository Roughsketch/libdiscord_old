#pragma once

#include <string>

#include "common.h"

namespace ModDiscord
{
  namespace API
  {
    enum RequestType : uint8_t
    {
      GET,
      POST
    };

    std::string get_wss_url();
  }
}