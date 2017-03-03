#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class User;
  enum ChannelType : uint8_t;

  class InviteMetadata
  {
    std::shared_ptr<User> m_invitor;
    uint32_t m_uses;
    uint32_t m_max_uses;
    uint32_t m_max_age;
    bool m_temporary;
    uint32_t m_created_at;
    bool m_revoked;
  public:
    InviteMetadata();
    explicit InviteMetadata(nlohmann::json data);
  };

  class InviteGuild : public Identifiable
  {
    std::string m_name;
    std::string m_splash;
    std::string m_icon;
  public:
    InviteGuild();
    explicit InviteGuild(nlohmann::json data);
  };

  class InviteChannel : public Identifiable
  {
    std::string m_name;
    ChannelType m_type;
  public:
    InviteChannel();
    explicit InviteChannel(nlohmann::json data);
  };

  class Invite
  {
    std::string m_code;
    std::shared_ptr<InviteGuild> m_guild;
    std::shared_ptr<InviteChannel> m_channel;
  public:
    Invite();
    explicit Invite(nlohmann::json data);
  };
}