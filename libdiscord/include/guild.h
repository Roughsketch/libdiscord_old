#pragma once

#include <vector>

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class Channel;
  class Emoji;
  class Member;
  class Overwrite;
  class Permission;
  class PresenceUpdate;
  class Role;
  class User;
  class VoiceState;

  enum class VerificationLevel
  {
    None,
    Low,
    Medium,
    High
  };

  inline void from_json(const nlohmann::json& json, VerificationLevel& level)
  {
    level = static_cast<VerificationLevel>(json.get<int>());
  }

  inline void to_json(nlohmann::json& json, const VerificationLevel& level)
  {
    json = static_cast<int>(level);
  }

  enum class NotificationLevel
  {
    None,
    MentionOnly
  };

  inline void from_json(const nlohmann::json& json, NotificationLevel& level)
  {
    level = static_cast<NotificationLevel>(json.get<int>());
  }

  inline void to_json(nlohmann::json& json, const NotificationLevel& level)
  {
    json = static_cast<int>(level);
  }

  class Guild : public Identifiable
  {
    std::string m_name;
    std::string m_icon;
    std::string m_splash;
    Snowflake m_owner_id;
    std::string m_region;
    Snowflake m_afk_channel_id;
    uint32_t m_afk_timeout;
    bool m_embed_enabled;
    Snowflake m_embed_channel_id;
    VerificationLevel m_verification_level;
    NotificationLevel m_default_message_notifications;
    std::vector<std::shared_ptr<Role>> m_roles;
    std::vector<std::shared_ptr<Emoji>> m_emojis;
    std::vector<std::string> m_features;
    uint32_t m_mfa_level;
    std::string m_joined_at;
    bool m_large;
    uint32_t m_member_count;
    std::vector<std::shared_ptr<VoiceState>> m_voice_states;
    std::map<Snowflake, std::shared_ptr<Member>> m_members;
    std::vector<std::shared_ptr<Channel>> m_channels;
    std::map<Snowflake, std::shared_ptr<PresenceUpdate>> m_presences;

    bool m_unavailable;
  public:
    Guild();
    explicit Guild(const nlohmann::json& data);

    /** Merge the values of another guild object into this one.
     
        @param other The guild whose values should replace the current guild's.
     */
    void merge(std::shared_ptr<Guild> other);

    /** Get the name of a Guild
     
        @return The name of the Guild.
     */
    std::string name() const;

    /** Get the region of a guild.
     
        @return The region of a guild.
     */
    std::string region() const;

    /** Get the verification level of a guild.
     
        @return The verification level of the guild.
     */
    VerificationLevel verification_level() const;

    /** Get the default notification level of a guild.
     
        @return The current default notification level of this guild.
     */
    NotificationLevel notification_level() const;

    /** Get the AFK channel's id.
     
        @return The guild's AFK channel id.
     */
    Snowflake afk_channel() const;

    /** Get the guild's AFK timeout.
     
        @return The guild's AFK timeout in seconds.
     */
    uint32_t afk_timeout() const;

    /** Get the guild's owner id.
     
        @return The guild's owner id.
     */
    Snowflake owner_id() const;

    /** Get the owner of a guild.
     
        @return The owner of the guild.
     */
    std::shared_ptr<User> owner() const;

    /** Get the name of a Guild

        @return A vector of Emoji that the guild owns.
    */
    std::vector<std::shared_ptr<Emoji>> emojis() const;

    /** Get the member count of a guild.
     
        @return The amount of members in this guild.
     */
    uint32_t member_count() const;

    /** Get a user in this guild.
     
        @param user_id The id of the user to get.
        @return The user that was found.
     */
    std::shared_ptr<User> get_user(Snowflake user_id) const;

    /** Set the name of this guild.

    NOTE: This has no outside effect unless done within a modify callback.

    @param name The new name of the guild.
    */
    void set_name(std::string name);

    /** Set the region of this guild.

        NOTE: This has no outside effect unless done within a modify callback.

        @param region The new region of the guild.
    */
    void set_region(std::string region);

    /** Set the verification level of this channel.

    NOTE: This has no outside effect unless done within a modify callback.

    @param level The new verification level of the guild.
    */
    void set_verification_level(VerificationLevel level);

    /** Set the notification level of this guild.

    NOTE: This has no outside effect unless done within a modify callback.

    @param level The new notification level of the guild.
    */
    void set_notification_level(NotificationLevel level);

    /** Set the afk channel for this guild.

    NOTE: This has no outside effect unless done within a modify callback.

    @param channel_id The new afk channel for the guild.
    */
    void set_afk_channel(Snowflake channel_id);

    /** Set the afk timeout for this guild.

        NOTE: This has no outside effect unless done within a modify callback.

        @param timeout The new afk timeout for the guild in seconds.
    */
    void set_afk_timeout(uint32_t timeout);

    /** Set the new owner of this guild. Must be the owner of the guild for this to have any effect.

    NOTE: This has no outside effect unless done within a modify callback.

    @param user_id The new owner of the guild.
    */
    void set_owner(Snowflake user_id);


    /** Update the list of emoji that are available on the guild.

        @param emojis A vector of Emoji that the server owns.
    */
    void set_emojis(std::vector<std::shared_ptr<Emoji>> emojis);

    /** Sets a guild as unavailable.

        @param unavailable Whether or not the guild is unavailable.
    */
    void set_unavailable(bool unavailable);

    /** Adds a member to the guild's list of members.

        @param member The member that was added.
    */
    void add_member(std::shared_ptr<Member> member);

    /** Removes a member to the guild's list of members.

        @param member The member that was removed.
    */
    void remove_member(std::shared_ptr<Member> member);

    /** Update the data of a member in the guild.

        @param roles A list of Role ids that the user has.
        @param user A User object that describes the user.
        @param nick The user's current nickname.
    */
    void update_member(std::vector<Snowflake> roles, std::shared_ptr<User> user, std::string nick);

    /** Adds a role to the guild's list of roles.

        @param role A Role object that describes the role that was added.
    */
    void add_role(Role role);

    /** Removes a role to the guild's list of roles.

        @param role The id of the role to remove.
    */
    void remove_role(Snowflake role);

    /** Updates a role in the guild's list of roles.

        If this method cannot find a role with the same id as the role passed in, then
        it will ignore the update. This method only updates roles when the id already exists.

        @param role A Role object that already exists in the guild, but has updated values.
    */
    void update_role(Role role);

    /** Adds a channel to a guild.
     
        @param channel The channel to add.
     */
    void add_channel(std::shared_ptr<Channel> channel);

    /** Removes a channel from a guild.
     
        @param channel The channel to remove.
     */
    void remove_channel(std::shared_ptr<Channel> channel);

    /** Updates a presence object in a guild.
     
        @param presence The presence to update.
     */
    void update_presence(std::shared_ptr<PresenceUpdate> presence);

    /** Find a channel by name.
     
        @param name The name of the channel to find.
     */
    std::shared_ptr<Channel> find_channel(std::string name);

    /* *********************
    *
    * API Calls start here.
    *
    * ********************/


    /** Modify a guild's attributes. Attributes must be changed in the callback.
     
        @code
        guild = guild->modify([](std::shared_ptr<Guild> old) {
          old->set_name("C++ Fanboys");
          old->set_verification_level(VerificationLevel::Low);
        });
        @endcode
     
        @param modify_block The block that will modify the contents of the guild.
        @return The modified guild object.
     */
    std::shared_ptr<Guild> modify(std::function<void(std::shared_ptr<Guild>)> modify_block) const;

    /** Removes a guild. Cannot be undone.
     
        @return The guild that was removed.
     */
    std::shared_ptr<Guild> remove() const;

    /** Get a list of this guild's channels.
     
        @return A list of channels this guild owns.
     */
    std::vector<std::shared_ptr<Channel>> channels() const;

    /** Create a new text channel in this guild.

        @param name The name of the new channel.
        @param permission_overwrites The permission overwrites this channel should inherit.
        @return The channel that was created.
    */
    std::shared_ptr<Channel> create_text_channel(std::string name, std::vector<std::shared_ptr<Overwrite>> permission_overwrites = {}) const;

    /** Create a new voice channel in this guild.

        @param name The name of the new channel.
        @param bitrate The bitrate for this channel.
        @param user_limit The user limit for this channel.
        @param permission_overwrites The permission overwrites this channel should inherit.
        @return The channel that was created.
    */
    std::shared_ptr<Channel> create_voice_channel(std::string name, uint32_t bitrate, uint32_t user_limit, std::vector<std::shared_ptr<Overwrite>> permission_overwrites = {}) const;

    /** Change the order of channels in the guild.
     
        @param positions A map of channel ids to positions that will be used to reorder them.
        @return The list of channels in the guild.
     */
    std::vector<std::shared_ptr<Channel>> reorder_channels(const std::map<Snowflake, uint32_t>& positions) const;
  };

  inline void from_json(const nlohmann::json& json, Guild& guild)
  {
    guild = Guild(json);
  }

  class UserGuild : public Identifiable
  {
    std::string m_name;
    std::string m_icon;
    bool m_owner;
    std::shared_ptr<Permission> m_permissions;
  public:
    UserGuild();
    explicit UserGuild(const nlohmann::json& data);
    
    /** Get the name of this guild.
     
        @return The name of this guild object.
     */
    std::string name() const;

    /** Get the icon hash of this guild.
     
        @return The icon hash of this guild.
     */
    std::string icon() const;

    /** Get owner status of this guild.
     
        @return True if the current user is the owner of this guild.
     */
    bool owner() const;
    
    /** Get the permissions set for this user on this guild.
     
        @return Current user's permissions in this guild.
     */
    std::shared_ptr<Permission> permissions() const;
  };
  
  class GuildEmbed
  {
    bool m_enabled;
    Snowflake m_channel_id;
  public:
    GuildEmbed();
    explicit GuildEmbed(const nlohmann::json& data);

    /** Get whether this embed is enabled or not.
    
        @return Whether this embed is enabled.
     */
    bool enabled() const;

    /** Get the channel id for this embed.
     
        @return The channel id for this embed.
     */
    Snowflake channel_id() const;
  };
}
