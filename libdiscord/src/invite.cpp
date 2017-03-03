#include "invite.h"

#include "channel.h"
#include "user.h"

namespace Discord
{
  InviteMetadata::InviteMetadata()
  {
    m_uses = 0;
    m_max_uses = 0;
    m_max_age = 0;
    m_temporary = false;
    m_created_at = 0;
    m_revoked = false;
  }

  InviteMetadata::InviteMetadata(nlohmann::json data)
  {
    set_from_json(m_invitor, "invitor", data);
    set_from_json(m_uses, "uses", data);
    set_from_json(m_max_uses, "max_uses", data);
    set_from_json(m_max_age, "max_age", data);
    set_from_json(m_temporary, "temporary", data);
    set_from_json(m_created_at, "created_at", data);
    set_from_json(m_revoked, "revoked", data);
  }

  InviteGuild::InviteGuild()
  {
  }

  InviteGuild::InviteGuild(nlohmann::json data)
  {
    set_from_json(m_name, "name", data);
    set_from_json(m_splash, "splash", data);
    set_from_json(m_icon, "icon", data);
  }

  InviteChannel::InviteChannel()
  {
    m_type = Text;
  }

  InviteChannel::InviteChannel(nlohmann::json data)
  {
    set_from_json(m_name, "name", data);
    set_from_json(m_type, "type", data);
  }

  Invite::Invite()
  {
  }

  Invite::Invite(nlohmann::json data)
  {
    set_from_json(m_code, "code", data);
    set_from_json(m_guild, "guild", data);
    set_from_json(m_channel, "channel", data);
  }
}
