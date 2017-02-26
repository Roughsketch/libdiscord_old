#pragma once

#include <map>

#include "guild.h"

namespace ModDiscord
{
  namespace API
  {
    namespace Guild
    {
      static std::map<snowflake, std::shared_ptr<ModDiscord::Guild>> GuildCache;

      void update_cache(std::shared_ptr<ModDiscord::Guild> guild);
      void remove_cache(snowflake guild_id);
      void remove_cache(std::shared_ptr<ModDiscord::Guild> guild);
      void mark_unavailable(snowflake guild_id);

      std::shared_ptr<ModDiscord::Guild> get_guild(snowflake guild_id);
    }
  }
}
