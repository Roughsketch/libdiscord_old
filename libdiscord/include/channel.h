#pragma once

#include "common.h"
#include "identifiable.h"

namespace ModDiscord
{
  enum SearchCriteria
  {
    None,
    Before,
    After,
    Around
  };

  class Guild;
  class Message;
  class User;

  class Overwrite : public Identifiable
  {
    std::string m_type;
    uint32_t m_allow;
    uint32_t m_deny;
  public:
    Overwrite();
    explicit Overwrite(const nlohmann::json& data);
  };

  inline void from_json(const nlohmann::json& json, Overwrite& overwrite)
  {
    overwrite = Overwrite(json);
  }

  enum ChannelType : uint8_t
  {
    Text = 0,
    Private,
    Voice,
    Group
  };

  class Channel : public Identifiable
  {
    static const uint32_t MinNameSize = 2;
    static const uint32_t MaxNameSize = 100;
    static const uint32_t MaxTopicSize = 1024;
    static const uint32_t MinBitRate = 8000;
    static const uint32_t MaxBitRate = 96000;
    static const uint32_t MaxVIPBitRate = 128000;
    static const uint32_t MaxUserLimit = 99;

    //  All Channels
    bool m_is_private;
    Snowflake m_last_message_id;

    //  Guild Channels
    Snowflake m_guild_id;
    std::string m_name;
    ChannelType m_type;
    uint32_t m_position;
    std::vector<Overwrite> m_permission_overwrites;
    std::string m_topic;
    uint32_t m_bitrate;
    uint32_t m_user_limit;

    //  DM Specific
    std::shared_ptr<User> m_recipient;

    bool m_is_dm;
  public:
    Channel();
    explicit Channel(const nlohmann::json& data);

    /** Merge the values of another channel object into this one.

        @param other The channel whose values should replace the current channel's.
    */
    void merge(std::shared_ptr<Channel> other);

    /** Whether or not this channel is a DM.
     
        @return true if this channel is a DM.
     */
    bool is_dm() const;

    /** Whether or not this channel is a PM. Alias of is_dm.

        @return true if this channel is a PM.
    */
    bool is_pm() const;

    /** Get the guild id associated with this channel.
        
        @return The id of the guild that this channel belongs to.
     */
    Snowflake guild_id() const;

    /** Get the guild associated with this channel.

        @return The guild that this channel belongs to.
    */
    std::shared_ptr<Guild> guild() const;

    /** Get the name of this channel.

        @return The name of this channel.
    */
    std::string name() const;

    /** Get the type of channel this is. Look at the ChannelType enumeration for values.

        @return The type of channel this is.
    */
    ChannelType type() const;

    /** The raw position of the channel. Do not rely on this as an absolute index of position.

        @return The raw position of the channel.
    */
    uint32_t position() const;

    /** Get the channel's topic.

        If this channel is not a text channel, then this value should be empty.
        However, text channels may have empty topics as well, so use type() to 
        check what kind of channel this is before relying on this value.

        @return The topic of the channel. 
    */
    std::string topic() const;

    /** Get the channel's bitrate.

        If this channel is not a voice channel, then this value should be 0.

        @return The channel's bitrate. 
    */
    uint32_t bitrate() const;

    /** Get the user limit for this channel.

        If this channel is not a voice channel, then this value should be 0.
        If this channel is a voice channel, it should be between 1 and 99.

        @return The channel's user limit. 
    */
    uint32_t user_limit() const;

    /** Set the name of this channel.
     
        NOTE: This has no outside effect unless done within a modify callback.
     
        @param name The new name of the channel. Must be between 2 and 100 characters inclusive.
     */
    void set_name(std::string name);

    /** Set the topic of this channel.

        NOTE: This has no outside effect unless done within a modify callback.

        @param topic The new topic of the channel. Can be at most 1024 characters.
    */
    void set_topic(std::string topic);

    /** Set the raw position of this channel.

        NOTE: This has no outside effect unless done within a modify callback.

        @param position The new raw position of the channel.
    */
    void set_position(uint32_t position);

    /** Set the bitrate of this channel.

        NOTE: This has no outside effect unless done within a modify callback.

        @param bitrate The new bitrate of the channel. Must be between 8000 and 96000 inclusive.
    */
    void set_bitrate(uint32_t bitrate);

    /** Set the user limit of this channel.

        NOTE: This has no outside effect unless done within a modify callback.

        @param user_limit The new user limit of the channel. Must be between 1 and 99 inclusive.
    */
    void set_user_limit(uint32_t user_limit);



    /* *********************
     * 
     * API Calls start here.
     * 
     * ********************/


    /** Modify a channel.

        @code
        channel->modify([](std::shared_ptr<Channel> chan) {
            chan->set_name("example");
            chan->set_topic("This is a modified topic.");
            chan->set_position(1);
        });
        @endcode

        @param modify_block The block to call before sending the modified data through the API.
    */
    std::shared_ptr<Channel> modify(std::function<void(std::shared_ptr<Channel>)> modify_block);

    /** Closes a channel. Cannot be undone.
     
        @return The channel that was closed.
     */
    std::shared_ptr<Channel> close() const;

    /** Get a list of messages from this channel.
    
        When using a search method with this method, it is required that a message id is passed in as well.
        Leaving the message id blank with a search parameter will result in the function acting like no search
        was passed in at all.

        @param limit The amount of messages to get back. Max of 100.
        @param method The method to use when getting messages.
        @param message_id The id that the search will be based on if a search method was provided.
     */
    std::vector<std::shared_ptr<Message>> get_messages(int32_t limit = 50, SearchCriteria method = None, Snowflake message_id = 0) const;

    /** Get a particular message from a channel by its id.
     
        @param message_id The id of the message to get.
        @return The message that was retrieved.
     */
    std::shared_ptr<Message> get_message(Snowflake message_id) const;

    /** Send a message to a channel.
     
        @param content The message to send
        @param tts Whether or not this message should be text to speech.
        @return A shared pointer to the message that was sent.
     */
    std::shared_ptr<Message> send_message(std::string content, bool tts = false) const;

    /** Send a message to a channel that is deleted after a certain amount of time.
     
        @param content The message to send
        @param timeout The amount of time in seconds to wait before deleting the message.
        @param tts Whether or not this message should be text to speech.
        @return A shared pointer to the message that was sent.
     */
    void send_temp_message(std::string content, uint32_t timeout, bool tts = false) const;

    /** React to a message with an emoji.
     
        @param message_id The id of the message to react to.
        @param emoji The emoji to react with.
        @return Success status.
     */
    bool add_reaction(Snowflake message_id, std::shared_ptr<Emoji> emoji) const;

    /** Removes a reaction. If a user is specified, removes a reaction made by that user.
     
        @param message_id The id of the message to remove the reaction from.
        @param emoji The emoji that will be removed.
        @param user_id The user whose reaction will be removed.
        @return Success status.
     */
    bool remove_reaction(Snowflake message_id, std::shared_ptr<Emoji> emoji, Snowflake user_id = 0) const;

    /** Get a list of users who reacted to a message with a certain emoji.
     
        @param message_id The id of the message to get reactions from.
        @param emoji The emoji to get the user list from.
        @return A list of users who reacted to the message with the given emoji.
     */
    std::vector<std::shared_ptr<User>> get_reactions(Snowflake message_id, std::shared_ptr<Emoji> emoji) const;

    /** Removes all reactions on a message.
     
        @param message_id The message that will have all reactions removed.
     */
    void remove_all_reactions(Snowflake message_id) const;

    /** Edit a message with new content.
     
        @param message_id The message to edit.
        @param new_content The content to edit into message.
        @return The message that was edited.
     */
    std::shared_ptr<Message> edit_message(Snowflake message_id, std::string new_content) const;

    /** Remove a message.
     
        @param message_id The message to delete.
        @return Success status.
     */
    bool remove_message(Snowflake message_id) const;

    /** Bulk remove messages.
     
        The list of ids must be of a length between 2 and 100 inclusive. If this isn't met, the command won't be invoked.

        @param message_ids A list of messages to remove.
        @return Success status.
     */
    bool remove_messages(std::vector<Snowflake> message_ids) const;
  };

  inline void from_json(const nlohmann::json& json, Channel& channel)
  {
    channel = Channel(json);
  }
}
