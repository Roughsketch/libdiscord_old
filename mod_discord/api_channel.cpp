#include "api.h"
#include "api_channel.h"

namespace ModDiscord
{
  namespace API
  {
    namespace Channel
    {
      ModDiscord::Channel get_channel(snowflake channel_id)
      {
        auto json = request(GET, "channels/" + std::to_string(channel_id));

        if (!json.empty())
        {
          return ModDiscord::Channel(json);
        }
        
        return ModDiscord::Channel();
      }

      nlohmann::json create_message(snowflake channel_id, std::string content)
      {
        return request(POST, "channels/" + std::to_string(channel_id) + "/messages", {
          { "content", content },
          { "mentions", nlohmann::json::array() },
          { "tts", false },
          { "embed", nullptr }
        });
      }
    }
  }
}
