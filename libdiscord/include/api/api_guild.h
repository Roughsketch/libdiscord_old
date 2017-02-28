#pragma once

namespace ModDiscord
{
  class Guild;

  namespace API
  {
    namespace Guild
    {
      /** Updates the values of a Guild object in the cache.
         
         @param guild A shared pointer to the guild to update.
       */
      std::shared_ptr<ModDiscord::Guild> update_cache(std::shared_ptr<ModDiscord::Guild> guild);

      /** Removes a Guild from the cache given its id.

          @param guild_id A Snowflake set to the guild's id.
      */
      void remove_cache(Snowflake guild_id);

      /** Removes a Guild from the cache given its pointer.

          @param guild A shared pointer to the guild to remove.
      */
      void remove_cache(std::shared_ptr<ModDiscord::Guild> guild);

      /** Sets a Guild's unavailable flag.

          @param guild_id A Snowflake set to the guild's id.
      */
      void mark_unavailable(Snowflake guild_id);

      /** Get a Guild object from the cache, or request it from the API if it is not stored.

          @param guild_id A Snowflake set to the guild's id.
          @return A shared pointer to the Guild or nullptr on failure.
      */
      std::shared_ptr<ModDiscord::Guild> get_guild(Snowflake guild_id);
    }
  }
}
