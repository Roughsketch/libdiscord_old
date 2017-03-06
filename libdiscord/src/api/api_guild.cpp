#include "api.h"
#include "api/api_guild.h"
#include "channel.h"
#include "guild.h"
#include "integration.h"
#include "invite.h"
#include "member.h"
#include "role.h"
#include "user.h"
#include "voice.h"

#include <map>

namespace Discord
{
  namespace API
  {
    namespace Guild
    {
      static std::map<Snowflake, std::shared_ptr<Discord::Guild>> GuildCache;

      std::shared_ptr<Discord::Guild> update_cache(std::shared_ptr<Discord::Guild> guild)
      {
        if (GuildCache.count(guild->id()))
        {
          LOG(TRACE) << "Merging new guild information with cached value.";
          auto old = get(guild->id());
          old->merge(guild);
        }
        else
        {
          GuildCache[guild->id()] = guild;
        }

        return guild;
      }

      void remove_cache(Snowflake guild_id)
      {
        auto itr = GuildCache.find(guild_id);

        if (itr != std::end(GuildCache))
        {
          GuildCache.erase(itr);
        }
        else
        {
          LOG(ERROR) << "Attempted to delete a guild that wasn't in the cache.";
        }
      }

      void remove_cache(std::shared_ptr<Discord::Guild> guild)
      {
        remove_cache(guild->id());
      }

      void mark_unavailable(Snowflake guild_id)
      {
        auto guild = get(guild_id);
        guild->set_unavailable(true);
      }

      std::shared_ptr<Discord::Guild> get(Snowflake guild_id)
      {
        if (GuildCache.count(guild_id))
        {
          return GuildCache[guild_id];
        }

        LOG(DEBUG) << "Could not return guild from cache, calling API.";

        //auto json = request(Guilds, guild_id, GET, "guilds/" + guild_id.to_string());
        auto response = request(APICall(guild_id) << "guilds" << guild_id, GET);

        if (!response.empty())
        {
          auto guild = std::make_shared<Discord::Guild>(response);
          update_cache(guild);
          return guild;
        }

        LOG(ERROR) << "Could not get Guild object with id " << guild_id.to_string();
        return nullptr;
      }

      std::shared_ptr<Discord::Guild> modify(Snowflake guild_id, std::shared_ptr<Discord::Guild> guild)
      {
        nlohmann::json payload = {
          { "name", guild->name() },
          { "region", guild->region() },
          { "verification_level", guild->verification_level() },
          { "default_message_notifications", guild->notification_level() },
          { "afk_channel_id", guild->afk_channel() },
          { "afk_timeout", guild->afk_timeout() },
          { "owner_id", guild->owner_id() }
        };

        //auto response = request(Guilds, guild_id, PATCH, "guilds/" + guild->id().to_string(), payload);
        auto response = request(APICall(guild_id) << "guilds" << guild->id(), PATCH, payload);

        return response;
      }

      std::shared_ptr<Discord::Guild> remove(Snowflake guild_id)
      {
        auto response = request(APICall() << "guilds" << guild_id, DEL);

        return response;
      }

      std::vector<std::shared_ptr<Discord::Channel>> get_channels(Snowflake guild_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "channels", GET);

        return response;
      }

      std::shared_ptr<Discord::Channel> create_text_channel(Snowflake guild_id, std::string name, std::vector<std::shared_ptr<Overwrite>> permission_overwrites)
      {
        auto response = request(APICall() << "guilds" << guild_id << "channels", POST, {
          { "name", name },
          { "type", "text" },
          { "permission_overwrites", permission_overwrites }
        });

        return response;
      }

      std::shared_ptr<Discord::Channel> create_voice_channel(Snowflake guild_id, std::string name, uint32_t bitrate, uint32_t user_limit, std::vector<std::shared_ptr<Overwrite>> permission_overwrites)
      {
        auto response = request(APICall() << "guilds" << guild_id << "channels", POST, {
          { "name", name },
          { "type", "text" },
          { "bitrate", bitrate },
          { "user_limit", user_limit }
        });

        return response;
      }

      std::vector<std::shared_ptr<Discord::Channel>> modify_channel_positions(Snowflake guild_id, const std::map<Snowflake, uint32_t>& positions)
      {
        nlohmann::json payload = {};
        
        for (auto& pair : positions)
        {
          payload[pair.first.to_string()] = pair.second;
        }

        auto response = request(APICall() << "guilds" << guild_id << "channels", PATCH, payload);

        return response;
      }

      std::shared_ptr<Member> get_member(Snowflake guild_id, Snowflake user_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "members" << user_id, GET);

        return response;
      }

      std::vector<std::shared_ptr<Member>> get_members(Snowflake guild_id, uint32_t limit, Snowflake after)
      {
        auto response = request(APICall() << "guilds" << guild_id << "members", GET, {
          { "limit", limit },
          { "after", after }
        });

        return response;
      }

      bool add_member(Snowflake guild_id, Snowflake user_id, std::string access_token, std::string nick, std::vector<std::shared_ptr<Role>> roles, bool muted, bool deafened)
      {
        auto response = request(APICall() << "guilds" << guild_id << "members" << user_id, PUT, {
          { "access_token", access_token },
          { "nick", nick },
          { "roles", roles },
          { "mute", muted },
          { "deaf", deafened }
        });

        return response.get<int>() == Status::Created;
      }

      bool modify_member(Snowflake guild_id, Snowflake user_id, std::string nick, std::vector<Role> roles, bool muted, bool deafened, Snowflake channel_id)
      {
        nlohmann::json payload = {
          { "nick", nick },
          { "roles", roles },
          { "mute", muted },
          { "deaf", deafened }
        };

        if (channel_id != 0)
        {
          payload["channel_id"] = channel_id;
        }

        auto response = request(APICall() << "guilds" << guild_id << "members" << user_id, PATCH, payload);

        return response.get<int>() == Status::Created;
      }

      bool set_nickname(Snowflake guild_id, std::string nick)
      {
        auto response = request(APICall() << "guilds" << guild_id << "members/@me/nick", PATCH, { { "nick", nick } });

        return response.get<int>() == Status::OK;
      }

      bool add_member_role(Snowflake guild_id, Snowflake user_id, Snowflake role_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "members" << user_id << "roles" << role_id, PUT);

        return response.get<int>() == Status::NoContent;
      }

      bool remove_member_role(Snowflake guild_id, Snowflake user_id, Snowflake role_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "members" << user_id << "roles" << role_id, DEL);

        return response.get<int>() == Status::NoContent;
      }

      bool remove_member(Snowflake guild_id, Snowflake user_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "members" << user_id, DEL);

        return response.get<int>() == Status::NoContent;
      }

      std::vector<std::shared_ptr<User>> get_bans(Snowflake guild_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "bans", GET);

        return response;
      }

      bool ban(Snowflake guild_id, Snowflake user_id, uint32_t delete_x_days)
      {
        if (delete_x_days > 7)
        {
          delete_x_days = 7;
        }

        auto response = request(APICall() << "guilds" << guild_id << "bans" << user_id, PUT, { {"delete-message-days", delete_x_days} });

        return response.get<int>() == Status::NoContent;
      }

      bool unban(Snowflake guild_id, Snowflake user_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "bans" << user_id, DEL);

        return response.get<int>() == Status::NoContent;
      }

      std::vector<std::shared_ptr<Role>> get_roles(Snowflake guild_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "roles", GET);

        return response;
      }

      std::shared_ptr<Role> create_role(Snowflake guild_id, std::string name, std::shared_ptr<Permission> permissions, uint32_t rgb_color, bool hoist, bool mentionable)
      {
        auto response = request(APICall() << "guilds" << guild_id << "roles", POST, {
          { "name", name },
          { "permissions", permissions },
          { "color", rgb_color },
          { "hoist", hoist },
          { "mentionable", mentionable }
        });

        return response;
      }

      std::vector<std::shared_ptr<Role>> modify_role_positions(Snowflake guild_id, const std::map<Snowflake, uint32_t>& positions)
      {
        nlohmann::json payload = {};
        
        for (auto& pair : positions)
        {
          payload[pair.first.to_string()] = pair.second;
        }

        auto response = request(APICall() << "guilds" << guild_id << "roles", PATCH, payload);

        return response;
      }

      std::shared_ptr<Role> modify_role(Snowflake guild_id, Snowflake role_id, std::string name, std::shared_ptr<Permission> permissions, uint32_t rgb_color, bool hoist, bool mentionable)
      {
        auto response = request(APICall() << "guilds" << guild_id << "roles" << role_id, PATCH, {
          { "name", name },
          { "permissions", permissions },
          { "color", rgb_color },
          { "hoist", hoist },
          { "mentionable", mentionable }
        });

        return response;
      }

      bool remove_role(Snowflake guild_id, Snowflake role_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "roles" << role_id, DEL);

        return response.get<int>() == Status::NoContent;
      }

      uint32_t get_prune_count(Snowflake guild_id, uint32_t days)
      {
        auto response = request(APICall() << "guilds" << guild_id << "prune", GET, {
          { "days", days }
        });

        return response;
      }

      uint32_t prune(Snowflake guild_id, uint32_t days)
      {
        auto response = request(APICall() << "guilds" << guild_id << "prune", POST, {
          { "days", days }
        });

        return response["pruned"];
      }

      std::vector<std::shared_ptr<VoiceRegion>> get_voice_regions(Snowflake guild_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "regions", GET);

        return response;
      }

      std::vector<std::shared_ptr<Invite>> get_invites(Snowflake guild_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "invites", GET);

        return response;
      }

      std::vector<std::shared_ptr<Integration>> get_integrations(Snowflake guild_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "integrations", GET);

        return response;
      }

      bool create_integration(Snowflake guild_id, std::string type, Snowflake integration_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "integrations", POST, {
          { "type", type },
          { "id", integration_id }
        });

        return response.get<int>() == Status::NoContent;
      }

      bool modify_integration(Snowflake guild_id, Snowflake integration_id, uint32_t expire_behavior, uint32_t expire_grace_period, bool enable_emoticons)
      {
        auto response = request(APICall() << "guilds" << guild_id << "integrations" << integration_id, PATCH, {
          { "expire_behavior", expire_behavior },
          { "expire_grace_period", expire_grace_period },
          { "enable_emoticons", enable_emoticons }
        });

        return response.get<int>() == Status::NoContent;
      }

      bool remove_integration(Snowflake guild_id, Snowflake integration_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "integrations" << integration_id, DEL);

        return response.get<int>() == Status::NoContent;
      }

      bool sync_integration(Snowflake guild_id, Snowflake integration_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "integrations" << integration_id << "sync", POST);

        return response.get<int>() == Status::NoContent;
      }

      std::shared_ptr<GuildEmbed> get_embed(Snowflake guild_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "embed", GET);

        return response;
      }

      std::shared_ptr<GuildEmbed> modify_embed(Snowflake guild_id, bool enabled, Snowflake channel_id)
      {
        auto response = request(APICall() << "guilds" << guild_id << "embed", PATCH, {
          { "enabled", enabled },
          { "channel_id", channel_id }
        });

        return response;
      }
    }
  }
}
