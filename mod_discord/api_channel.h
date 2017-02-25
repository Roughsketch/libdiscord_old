#pragma once

#include "common.h"
#include "channel.h"

namespace ModDiscord
{
  namespace API
  {
    namespace Channel
    {
      ModDiscord::Channel get_channel(snowflake channel_id);
      nlohmann::json create_message(snowflake channel_id, std::string content);
    }
  }
}
