#pragma once

#include "common.h"

namespace ModDiscord
{
  class Channel;
  class Emoji;
  class Invite;
  class Message;
  class Overwrite;
  class User;

  namespace API
  {
    namespace Channel
    {
      enum SearchCriteria
      {
        None,
        Before,
        After,
        Around
      };

      /** Add or update a Channel in the cache.
       
          @param channel The channel to add or update.
       */
      void update_cache(std::shared_ptr<ModDiscord::Channel> channel);

      /** Remove a Channel from the cache.
          
          @param channel The channel to remove.
       */
      void remove_cache(std::shared_ptr<ModDiscord::Channel> channel);

      /** Get a channel from either the cache, or the API if it's not already cached.
       
          @param channel_id The id of the channel to retrieve.
          @return The channel object, or nullptr if it doesn't exist. 
       */
      std::shared_ptr<ModDiscord::Channel> get_channel(Snowflake channel_id);

      /** Modify a channel's attributes.
       
          @param channel_id The id of the channel to modify.
          @param name The new name of the channel. Must be between 2 and 100 characters inclusive.
          @param position The raw position of the channel. Does not directly correspond to the order.
          @param topic The new topic of the channel. Must be 1024 characters or less.
          @return The channel that was modified.
       */
      std::shared_ptr<ModDiscord::Channel> modify_text_channel(Snowflake channel_id, std::string name = "", uint32_t position = 0, std::string topic = "");

      /** Modify a channe's attributes.
       
          @param channel_id The id of the channel to modify.
          @param name The new name of the channel. Must be between 2 and 100 characters inclusive.
          @param position The raw position of the channel. Does not directly correspond to the order.
          @param bitrate The new bitrate of the channel. Must be between 8000 and 96000 inclusive.
          @param user_limit The new user limit for the channel. Must be between 1 and 99 inclusive.
          @return The channel that was modified.
       */
      std::shared_ptr<ModDiscord::Channel> modify_voice_channel(Snowflake channel_id, std::string name = "", uint32_t position = 0, uint32_t bitrate = 0, uint32_t user_limit = 0);

      /** Deletes a channel.
       
          @param channel_id The channel to delete
          @return The channel object that was deleted.
       */
      std::shared_ptr<ModDiscord::Channel> delete_channel(Snowflake channel_id);

      /** Gets a list of messages from a channel. Optionally accepts a search criteria and message id to search around.
       
          @param channel_id The channel to get messages from.
          @param limit The amount of messages to get. Defaults to 50 and must be between 1 and 100 inclusive.
          @param method The search method to use when getting methods. If used, must pass in the pivot Snowflake.
          @param pivot The message to base the search around.
          @return A vector of messages that were retrieved.
       */
      std::vector<std::shared_ptr<Message>> get_messages(Snowflake channel_id, int32_t limit = 50, SearchCriteria method = None, Snowflake pivot = 0);

      /** Get a single message from a channel.
       
          @param channel_id The channel to get the message from.
          @param message_id The message to get.
          @return The message that was retrieved.
       */
      std::shared_ptr<Message> get_message(Snowflake channel_id, Snowflake message_id);

      /** Creates a message and sends it to the channel.
       
          @param channel_id The channel to send the message to.
          @param content The content of the message.
          @param tts Whether or not this message should be text-to-speech.
          @return The message that was sent.
       */
      std::shared_ptr<Message> create_message(Snowflake channel_id, std::string content, bool tts = false);

      /** Creates a reaction on a message.
       
          @param channel_id The channel that holds the message to react to.
          @param message_id The message to react to.
          @param emoji The emoji to react with.
          @return Success status.
       */
      bool create_reaction(Snowflake channel_id, Snowflake message_id, std::shared_ptr<Emoji> emoji);

      /** Deletes a reaction that the bot has made.
       
          @param channel_id The channel where the message was reacted to.
          @param message_id The message to remove the reaction from.
          @param emoji The emoji to remove from the reaction list.
          @return Success status.
       */
      bool delete_own_reaction(Snowflake channel_id, Snowflake message_id, std::shared_ptr<Emoji> emoji);

      /** Deletes a reaction that a user has made.
       
          @param channel_id The channel where the message was reacted to.
          @param message_id The message to remove the reaction from.
          @param emoji The emoji to remove from the reaction list.
          @param user_id The id of the user whose reaction will be removed.
          @return Success status.
       */
      bool delete_user_reaction(Snowflake channel_id, Snowflake message_id, std::shared_ptr<Emoji> emoji, Snowflake user_id);

      /** Get a list of users who reacted with a particular emoji.
       
          @param channel_id The channel where the message was reacted to.
          @param message_id The message to get the list of reaction users from.
          @param emoji The emoji that we should get the user list for.
          @return A list of users who reacted with the given emoji.
       */
      std::vector<std::shared_ptr<User>> get_reactions(Snowflake channel_id, Snowflake message_id, std::shared_ptr<Emoji> emoji);

      /** Deletes all reactions on a message.
       
          @param channel_id The channel where the message was reacted to.
          @param message_id The message to delete all reactions on.
       */
      void delete_all_reactions(Snowflake channel_id, Snowflake message_id);

      /** Edits a message with new information.
       
          @param channel_id The channel where the message is located.
          @param message_id The message to edit.
          @param new_content The new content of the message.
          @return The message that was edited.
       */
      std::shared_ptr<Message> edit_message(Snowflake channel_id, Snowflake message_id, std::string new_content);

      /** Delete a message from a channel.
       
          @param channel_id The channel where the message is located.
          @param message_id The message to delete.
          @return Success status.
       */
      bool delete_message(Snowflake channel_id, Snowflake message_id);

      /** Delete a list of messages all at once.
       
          @param channel_id The channel where the messages are located.
          @param message_ids A list of message ids to delete. Must be 100 or fewer.
          @return Success status.
       */
      bool bulk_delete_messages(Snowflake channel_id, std::vector<Snowflake> message_ids);
      
      /** Edits the permissions of either a member or a role.
       
          @param channel_id The channel whose permissions will be updated.
          @param overwrite The overwrite to update.
          @param allow The permissions to allow.
          @param deny The permissions to deny.
          @return Success status.
       */
      bool edit_permissions(Snowflake channel_id, std::shared_ptr<Overwrite> overwrite, uint32_t allow, uint32_t deny, std::string type);

      /** Get a list of invites for this channel.
       
          @param channel_id The channel to get invites to.
          @return A list of invites to the given channel.
       */
      std::vector<std::shared_ptr<Invite>> get_channel_invites(Snowflake channel_id);

      /** Create a channel invite.
       
          @param channel_id The id of the channel to create an invite for.
          @param max_age The maximum lifetime for this invite.
          @param max_uses The maximum amount of time this invite can be used.
          @param temporary Whether or not this invite should be temporary. Defaults to false.
          @param unique Whether or not this invite is unique. Defaults to false.
       */
      std::shared_ptr<Invite> create_channel_invite(Snowflake channel_id, uint32_t max_age, uint32_t max_uses, bool temporary = false, bool unique = false);

      /** Delete permissions from a channel.
       
          @param channel_id The channel whose permissions to delete
          @param overwrite The permissions to delete.
       */
      bool delete_permission(Snowflake channel_id, std::shared_ptr<Overwrite> overwrite);

      /** Triggers a typing indicator for the current user.
       
          @param channel_id The channel to indicate typing in.
          @return Success status.
       */
      bool trigger_typing_indicator(Snowflake channel_id);

      /** Get a list of pinned messages in the given channel.
       
          @param channel_id The channel whose pinned messages to get.
          @return A list of messages that were pinned.
       */
      std::vector<std::shared_ptr<Message>> get_pinned_messages(Snowflake channel_id);
      
      /** Pins a message in a channel.
       
          @param channel_id The channel that owns the message to pin.
          @param message_id The message to pin.
          @return Success status.
       */
      bool add_pinned_message(Snowflake channel_id, Snowflake message_id);

      /** Remove a message from the list of pinned messages.
       
          @param channel_id The channel that owns the message to unpin.
          @param message_id The message to unpin.
          @return Success status.
       */
      bool delete_pinned_message(Snowflake channel_id, Snowflake message_id);
      
      /** Add a user to a group DM.
       
          @param channel_id The channel to add the user to.
          @param user_id The id of the user to add.
          @param access_token The access token that allows the bot to add a user to a DM.
          @param nickname The nickname of the added user.
       */
      void group_dm_add_recipient(Snowflake channel_id, Snowflake user_id, std::string access_token, std::string nickname);

      /** Remove a user from a group DM.
       
          @param channel_id The channel to remove the user from.
          @param user_id The id of the user to remove.
       */
      void group_dm_remove_recipient(Snowflake channel_id, Snowflake user_id);
    }
  }
}
