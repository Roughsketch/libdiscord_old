#pragma once

#include <chrono>
#include <vector>

#include "common.h"
#include "identifiable.h"

namespace ModDiscord
{
  class Channel;
  class Emoji;
  class Member;
  class PresenceUpdate;
  class Role;
  class User;
  class VoiceState;

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
    uint32_t m_verification_level;
    uint32_t m_default_message_notifications;
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
    explicit Guild(nlohmann::json data);

    /** Merge the values of another guild object into this one.
     
        @param other The guild whose values should replace the current guild's.
     */
    void merge(std::shared_ptr<Guild> other);

    /** Get the name of a Guild
     
        @return The name of the Guild.
     */
    std::string name() const;

    /** Get the name of a Guild

        @return A vector of Emoji that the guild owns.
    */
    std::vector<std::shared_ptr<Emoji>> emojis() const;

    /** Get the member count of a guild.
     
        @return The amount of members in this guild.
     */
    uint32_t member_count() const;

    std::shared_ptr<User> get_user(Snowflake user_id);

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
    void update_member(std::vector<Snowflake> roles, User user, std::string nick);

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

    /** Updates a presence object in a guild.
     
        @param presence The presence to update.
     */
    void update_presence(std::shared_ptr<PresenceUpdate> presence);
  };

  inline void from_json(const nlohmann::json& json, Guild& guild)
  {
    guild = Guild(json);
  }
}
