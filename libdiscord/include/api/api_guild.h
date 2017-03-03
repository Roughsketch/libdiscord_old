#pragma once

namespace Discord
{
  class Guild;

  namespace API
  {
    namespace Guild
    {
      /** Updates the values of a Guild object in the cache.
         
         @param guild A shared pointer to the guild to update.
       */
      std::shared_ptr<Discord::Guild> update_cache(std::shared_ptr<Discord::Guild> guild);

      /** Removes a Guild from the cache given its id.

          @param guild_id A Snowflake set to the guild's id.
      */
      void remove_cache(Snowflake guild_id);

      /** Removes a Guild from the cache given its pointer.

          @param guild A shared pointer to the guild to remove.
      */
      void remove_cache(std::shared_ptr<Discord::Guild> guild);

      /** Sets a Guild's unavailable flag.

          @param guild_id A Snowflake set to the guild's id.
      */
      void mark_unavailable(Snowflake guild_id);

      /** Get a Guild object from the cache, or request it from the API if it is not stored.

          @param guild_id A Snowflake set to the guild's id.
          @return A shared pointer to the Guild or nullptr on failure.
      */
      std::shared_ptr<Discord::Guild> get_guild(Snowflake guild_id);

      /** Modify the attributes of a guild.
       
          @param guild_id The guild to modify
          @return The modified guild object.
       */
      std::shared_ptr<Discord::Guild> modify_guild(Snowflake guild_id, std::shared_ptr<Discord::Guild> guild);
    }
  }
}
