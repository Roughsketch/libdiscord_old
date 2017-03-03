#pragma once

#include "common.h"
#include "identifiable.h"

namespace ModDiscord
{
  class Integration;

  class Connection
  {
    std::string m_id;
    std::string m_name;
    std::string m_type;
    bool m_revoked;
    std::vector<std::shared_ptr<Integration>> m_integrations;
  public:
  };

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
    explicit User(nlohmann::json data);

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
  };

  inline void from_json(const nlohmann::json& json, User& user)
  {
    user = User(json);
  }
}