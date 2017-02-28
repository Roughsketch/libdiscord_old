#pragma once

#include <chrono>
#include <vector>

#include "common.h"
#include "channel.h"
#include "emoji.h"
#include "events.h"
#include "identifiable.h"
#include "member.h"
#include "role.h"
#include "voice.h"

namespace ModDiscord
{
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
    std::vector<Role> m_roles;
    std::vector<Emoji> m_emojis;
    std::vector<std::string> m_features;
    uint32_t m_mfa_level;
    std::string m_joined_at;
    bool m_large;
    uint32_t m_member_count;
    std::vector<VoiceState> m_voice_states;
    std::vector<Member> m_members;
    std::vector<Channel> m_channels;
    std::vector<PresenceUpdate> m_presences;

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
    std::vector<Emoji> emojis() const;

    /** Update the list of emoji that are available on the guild.

        @param emojis A vector of Emoji that the server owns.
    */
    void set_emojis(std::vector<Emoji> emojis);

    /** Sets a guild as unavailable.

        @param unavailable Whether or not the guild is unavailable.
    */
    void set_unavailable(bool unavailable);

    /** Adds a member to the guild's list of members.

        @param member The member that was added.
    */
    void add_member(Member member);

    /** Removes a member to the guild's list of members.

        @param member The member that was removed.
    */
    void remove_member(Member member);

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
  };

  inline void from_json(const nlohmann::json& json, Guild& guild)
  {
    guild = Guild(json);
  }
}
