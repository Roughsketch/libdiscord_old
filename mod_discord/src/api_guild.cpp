#include "api.h"
#include "api/api_guild.h"
#include "guild.h"

#include <map>

namespace ModDiscord
{
  namespace API
  {
    namespace Guild
    {
      static std::map<Snowflake, std::shared_ptr<ModDiscord::Guild>> GuildCache;

      void update_cache(std::shared_ptr<ModDiscord::Guild> guild)
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

      void remove_cache(std::shared_ptr<ModDiscord::Guild> guild)
      {
        remove_cache(guild->id());
      }

      void mark_unavailable(Snowflake guild_id)
      {
        auto guild = get_guild(guild_id);
        guild->set_unavailable(true);
      }

      std::shared_ptr<ModDiscord::Guild> get_guild(Snowflake guild_id)
      {
        if (GuildCache.count(guild_id))
        {
          LOG(TRACE) << "Returning channel from cache.";
          return GuildCache[guild_id];
        }

        auto json = request(GET, "channels/" + guild_id.to_string());

        if (!json.empty())
        {
          auto guild = std::make_shared<ModDiscord::Guild>(json);
          update_cache(guild);
          return guild;
        }

        LOG(ERROR) << "Could not get Guild object with id " << guild_id.to_string();
        return nullptr;
      }
    }
  }
}