#include "api.h"
#include "api/api_guild.h"
#include "guild.h"

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
          auto old = get_guild(guild->id());
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
          LOG(ERROR) << "Attempted to delete a channel that wasn't in the cache.";
        }
      }

      void remove_cache(std::shared_ptr<Discord::Guild> guild)
      {
        remove_cache(guild->id());
      }

      void mark_unavailable(Snowflake guild_id)
      {
        auto guild = get_guild(guild_id);
        guild->set_unavailable(true);
      }

      std::shared_ptr<Discord::Guild> get_guild(Snowflake guild_id)
      {
        if (GuildCache.count(guild_id))
        {
          LOG(TRACE) << "Returning channel from cache.";
          return GuildCache[guild_id];
        }

        auto json = request(GetGuild, guild_id, GET, "channels/" + guild_id.to_string());

        if (!json.empty())
        {
          auto guild = std::make_shared<Discord::Guild>(json);
          update_cache(guild);
          return guild;
        }

        LOG(ERROR) << "Could not get Guild object with id " << guild_id.to_string();
        return nullptr;
      }

      std::shared_ptr<Discord::Guild> modify_guild(Snowflake guild_id, std::shared_ptr<Discord::Guild> guild)
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

        auto response = request(ModifyGuild, guild_id, PATCH, "guilds/" + guild->id().to_string(), payload);

        return response;
      }
    }
  }
}