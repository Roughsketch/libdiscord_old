#pragma once

#include "common.h"

namespace Discord
{
  class Channel;
  class Connection;
  class User;
  class UserGuild;

  namespace API
  {
    namespace User
    {
      /** Get information on the current user.
       
          @return The information on the current user.
       */
      std::shared_ptr<Discord::User> get_current_user();

      /** Get information on a specific user.
       
          @param user_id The id of the user to get information on.
          @return The information on the user.
       */
      std::shared_ptr<Discord::User> get_user(Snowflake user_id);

      /** Modifies the current user's username and optionally avatar.
       
          @param username The new username for this user.
          @param avatar The new avatar data for this user.
          @return The updated user.
       */
      std::shared_ptr<Discord::User> modify(std::string username, std::string avatar = "");

      /** Get the guilds this user is currently in.
       
          @return A list of UserGuild objects that identify the guilds this user is in.
       */
      std::vector<std::shared_ptr<UserGuild>> guilds(uint32_t limit = 100, SearchCriteria method = SearchCriteria::None, Snowflake guild_id = 0);

      /** Leaves a guild.
       
          @param guild_id The guild to leave.
          @return Success status.
       */
      bool leave_guild(Snowflake guild_id);

      /** Get a list of DMs that the current user is in.
       
          @return A list of DMs that the current user is in.
       */
      std::vector<std::shared_ptr<Discord::Channel>> get_dms();

      /** Creates a DM with another user.
       
          @param recipient_id The recipient for this DM.
          @return The channel that was created for this DM.
       */
      std::shared_ptr<Discord::Channel> create_dm(Snowflake recipient_id);

      /** Creates a group DM with a list of users.
       
          @param access_tokens Access tokens of users that have granted this user the gdm.join scope.
          @param user_nicknames A map of user ids to nicknames that will be assigned.
          @return The channel that was created for this group DM.
       */
      std::shared_ptr<Discord::Channel> create_group_dm(std::vector<std::string> access_tokens, std::map<Snowflake, std::string> user_nicknames);

      /** Get a list of connections that this user has.
       
          @return A list of connections for this user.
       */
      std::vector<std::shared_ptr<Connection>> connections();
    }
  }
}