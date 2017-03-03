#include "api.h"
#include "api/api_user.h"
#include "channel.h"
#include "user.h"

namespace ModDiscord
{
  namespace API
  {
    namespace User
    {
      std::shared_ptr<ModDiscord::User> get_current_user()
      {
        auto response = request(GetCurrentUser, 0, GET, "users/@me");

        return response;
      }

      std::shared_ptr<ModDiscord::User> get_user(Snowflake user_id)
      {
        auto response = request(GetUser, 0, GET, "users/" + user_id.to_string());

        return response;
      }

      std::shared_ptr<ModDiscord::User> modify(std::string username, std::string avatar)
      {
        nlohmann::json payload = {
          { "username", username }
        };

        auto response = request(ModifyCurrentUser, 0, PATCH, "users/@me", payload);

        return response;
      }

      bool leave_guild(Snowflake guild_id)
      {
        auto response = request(LeaveGuild, 0, DEL, "users/@me/guilds/" + guild_id.to_string());

        return response["response_status"].get<int>() == Status::NoContent;
      }

      std::vector<std::shared_ptr<ModDiscord::Channel>> get_dms()
      {
        auto response = request(GetUserDMs, 0, GET, "users/@me/channels");

        return response;
      }

      std::shared_ptr<ModDiscord::Channel> create_dm(Snowflake recipient_id)
      {
        auto response = request(CreateDM, 0, POST, "users/@me/channels", {
          { "recipient_id", recipient_id }
        });

        return response;
      }
    }
  }
}
