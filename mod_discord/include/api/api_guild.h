#pragma once

#include <map>

#include "guild.h"

namespace ModDiscord
{
  namespace API
  {
    namespace Guild
    {
      void update_cache(std::shared_ptr<ModDiscord::Guild> guild);
      void remove_cache(Snowflake guild_id);
      void remove_cache(std::shared_ptr<ModDiscord::Guild> guild);
      void mark_unavailable(Snowflake guild_id);

      std::shared_ptr<ModDiscord::Guild> get_guild(Snowflake guild_id);
    }
  }
}
