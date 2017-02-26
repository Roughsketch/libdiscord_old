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

      void update_cache(std::shared_ptr<ModDiscord::Channel> channel);
      void remove_cache(std::shared_ptr<ModDiscord::Channel> channel);

      std::shared_ptr<ModDiscord::Channel> get_channel(snowflake channel_id);
      std::shared_ptr<ModDiscord::Channel> modify_text_channel(snowflake channel_id, std::string name = "", uint32_t position = 0, std::string topic = "");
      std::shared_ptr<ModDiscord::Channel> modify_voice_channel(snowflake channel_id, std::string name = "", uint32_t position = 0, uint32_t bitrate = 0, uint32_t user_limit = 0);
      std::shared_ptr<Message> create_message(snowflake channel_id, std::string content, bool tts = false);
      bool delete_message(snowflake channel_id, snowflake message_id);
    }
  }
}
