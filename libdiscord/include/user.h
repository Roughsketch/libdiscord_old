#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class Integration;

  class User : public Identifiable
  {
    std::string m_username;
    std::string m_discriminator;
    std::string m_avatar;
    bool m_bot;
    bool m_mfa_enabled;
    bool m_verified;
    std::string m_email;
  public:
    User();
    explicit User(const nlohmann::json& data);

    /** Get the user's name.
     
        @return The user's name.
     */
    std::string username() const;

    /** Get the user's discriminator string.
     
        @return The user's discriminator string.
     */
    std::string discriminator() const;

    /** Get the user's distinct full name.
     
        @return The user's full name in the format username#discriminator
     */
    std::string distinct() const;
    
    /** Get the user's email address..
     
        @return The user's email address.
     */
    std::string email() const;

    /** Get the user's avatar id.
     
        @return The user's avatar id.
     */
    std::string avatar_id() const;

    /** Whether or not the user is a bot.
     
        @return true if user is a bot.
     */
    bool is_bot() const;

    /** Get a mention string for this user.
     
        @return A string that will mention the user.
     */
    std::string mention() const;

    /** Get a mention string for this user using their nickname.
     
        @return A string that will mention the user.
     */
    std::string mention_nick() const;
  };

  inline void from_json(const nlohmann::json& json, User& user)
  {
    user = User(json);
  }

  class Connection
  {
    std::string m_id;
    std::string m_name;
    std::string m_type;
    bool m_revoked;
    std::vector<std::shared_ptr<Integration>> m_integrations;
  public:
    Connection();
    explicit Connection(const nlohmann::json& data);

    /** Gets the id for this connection.
     
        @return The id of this connection.
     */
    std::string id() const;
    
    /** Get the name of this connection.
     
        @return The name of this connection.
     */
    std::string name() const;

    /** Gets the type of this connection (twitch, youtube)
     
        @return The type of this connection.
     */
    std::string type() const;

    /** Get the revoked status of this connection.
     
        @return The revoked status of this connection.
     */
    bool revoked() const;

    /** Gets the list of partial integrations associated with this connection.
     
        @return The list of partial integrations associated with this connection.
     */
    std::vector<std::shared_ptr<Integration>> integrations() const;
  };

}
