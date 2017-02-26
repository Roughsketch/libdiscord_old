#pragma once

#include "common.h"
#include "channel.h"

namespace ModDiscord
{
  namespace API
  {
    namespace Channel
    {
      static std::map<snowflake, std::shared_ptr<ModDiscord::Channel>> ChannelCache;

      std::shared_ptr<ModDiscord::Channel> get_channel(snowflake channel_id);
      std::shared_ptr<Message> create_message(snowflake channel_id, std::string content, bool tts = false);
      bool delete_message(snowflake channel_id, snowflake message_id);
    }
  }
}
