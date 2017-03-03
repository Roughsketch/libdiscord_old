#include "api.h"
#include "api/api_user.h"
#include "channel.h"
#include "user.h"

namespace Discord
{
  namespace API
  {
    namespace User
    {
      std::shared_ptr<Discord::User> get_current_user()
      {
        auto response = request(APICall() << "users/@me", GET);

        return response;
      }

      std::shared_ptr<Discord::User> get_user(Snowflake user_id)
      {
        auto response = request(APICall() << "users/" << user_id, GET);

        return response;
      }

      std::shared_ptr<Discord::User> modify(std::string username, std::string avatar)
      {
        nlohmann::json payload = {
          { "username", username }
        };

        auto response = request(APICall() << "users/@me", PATCH, payload);

        return response;
      }

      bool leave_guild(Snowflake guild_id)
      {
        auto response = request(APICall() << "users/@me/guilds/" << guild_id, DEL);

        return response["response_status"].get<int>() == Status::NoContent;
      }

      std::vector<std::shared_ptr<Discord::Channel>> get_dms()
      {
        auto response = request(APICall() << "users/@me/channels", GET);

        return response;
      }

      std::shared_ptr<Discord::Channel> create_dm(Snowflake recipient_id)
      {
        auto response = request(APICall() << "users/@me/channels", POST, {
          { "recipient_id", recipient_id }
        });

        return response;
      }

      std::shared_ptr<Discord::Channel> create_group_dm(std::vector<std::string> access_tokens, std::map<Snowflake, std::string> user_nicknames)
      {
        auto response = request(APICall() << "users/@me/channels", POST, {
          { "access_tokens", access_tokens },
          { "nicks", user_nicknames }
        });

        return response;
      }

      std::vector<std::shared_ptr<Connection>> connections()
      {
        auto response = request(APICall() << "users/@me/connections", GET);

        return response;
      }
    }
  }
}
