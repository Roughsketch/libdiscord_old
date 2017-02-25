#include "api.h"
#include "api_channel.h"

namespace ModDiscord
{
  namespace API
  {
    namespace Channel
    {
      std::shared_ptr<ModDiscord::Channel> get_channel(snowflake channel_id)
      {
        if (ChannelCache.count(channel_id))
        {
          BOOST_LOG_TRIVIAL(info) << "Returning channel from cache.";
          return ChannelCache[channel_id];
        }

        auto json = request(GET, "channels/" + std::to_string(channel_id));

        if (!json.empty())
        {
          return std::make_shared<ModDiscord::Channel>(json);
        }
        
        return std::make_shared<ModDiscord::Channel>();
      }

      std::shared_ptr<Message> create_message(snowflake channel_id, std::string content, bool tts)
      {
        auto response = request(POST, "channels/" + std::to_string(channel_id) + "/messages", {
          { "content", content },
          { "mentions", nlohmann::json::array() },
          { "tts", tts },
          { "embed", nullptr }
        });

        return std::make_shared<Message>(response);
      }

      bool delete_message(snowflake channel_id, snowflake message_id)
      {
        auto response = request(DELETE, "channels/" + std::to_string(channel_id) + "/messages/" + std::to_string(message_id));
        return response["response_status"].get<int>() == web::http::status_codes::NoContent;
      }
    }
  }
}
