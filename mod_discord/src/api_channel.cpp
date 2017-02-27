#include "api.h"
#include "api/api_channel.h"

#include <cpprest/http_msg.h>

namespace ModDiscord
{
  namespace API
  {
    namespace Channel
    {
      void update_cache(std::shared_ptr<ModDiscord::Channel> channel)
      {
        if (ChannelCache.count(channel->id()))
        {
          LOG(TRACE) << "Merging new channel information with cached value.";
          auto old = get_channel(channel->id());
          old->merge(channel);
        }
        else
        {
          ChannelCache[channel->id()] = channel;
        }
      }

      void remove_cache(std::shared_ptr<ModDiscord::Channel> channel)
      {
        auto itr = ChannelCache.find(channel->id());

        if (itr != std::end(ChannelCache))
        {
          ChannelCache.erase(itr);
        }
        else
        {
          LOG(ERROR) << "Attempted to delete a channel that wasn't in the cache.";
        }
      }

      std::shared_ptr<ModDiscord::Channel> get_channel(Snowflake channel_id)
      {
        if (ChannelCache.count(channel_id))
        {
          LOG(TRACE) << "Returning channel from cache.";
          return ChannelCache[channel_id];
        }

        auto json = request(GET, "channels/" + channel_id.to_string());

        if (!json.empty())
        {
          auto chan = std::make_shared<ModDiscord::Channel>(json);
          update_cache(chan);
          return chan;
        }

        LOG(ERROR) << "Could not get Channel object with id " << channel_id.to_string();
        return std::make_shared<ModDiscord::Channel>();
      }

      std::shared_ptr<ModDiscord::Channel> modify_text_channel(Snowflake channel_id, std::string name, uint32_t position, std::string topic)
      {
        auto chan = get_channel(channel_id);

        auto response = request(PATCH, "channels/" + channel_id.to_string(), {
          { "name", name },
          { "position", position },
          { "topic", topic }
        });

        if (response["response_status"].get<int>() != Status::OK)
        {
          LOG(ERROR) << "Modify text channel returned bad response (" << response["response_status"].get<int>() << ")";
        }

        return chan;
      }

      std::shared_ptr<ModDiscord::Channel> modify_voice_channel(Snowflake channel_id, std::string name, uint32_t position, uint32_t bitrate, uint32_t user_limit)
      {
        auto chan = get_channel(channel_id);

        auto response = request(PATCH, "channels/" + channel_id.to_string(), {
          { "name", name },
          { "position", position },
          { "bitrate", bitrate },
          { "user_limit", user_limit }
        });

        if (response["response_status"].get<int>() != Status::OK)
        {
          LOG(ERROR) << "Modify voice channel returned bad response (" << response["response_status"].get<int>() << ")";
        }

        return chan;
      }

      std::shared_ptr<Message> create_message(Snowflake channel_id, std::string content, bool tts)
      {
        auto response = request(POST, "channels/" + channel_id.to_string() + "/messages", {
          { "content", content },
          { "mentions", nlohmann::json::array() },
          { "tts", tts },
          { "embed", nullptr }
        });

        return std::make_shared<Message>(response);
      }

      bool delete_message(Snowflake channel_id, Snowflake message_id)
      {
        auto response = request(DEL, "channels/" + channel_id.to_string() + "/messages/" + message_id.to_string());
        return response["response_status"].get<int>() == Status::NoContent;
      }
    }
  }
}
