#pragma once

#include "common.h"
#include "role.h"
#include "user.h"

namespace ModDiscord
{
  namespace Event
  {
    class Message
    {
      snowflake m_id;
      snowflake m_channel_id;
      User m_author;
      std::string m_content;
      std::string m_timestamp;
      std::string m_edited_timestamp;
      bool m_tts;
      bool m_mention_everyone;
      std::vector<User> m_mentions;
      std::vector<Role> m_mention_roles;
      // std::vector<Attachment> m_attachments;
      // std::vector<Embed> m_embeds;
      // std::vector<Reaction> m_reactions;
      snowflake m_nonce;
      bool m_pinned;
      std::string m_webhook_id;
    public:
      Message();
      explicit Message(nlohmann::json data);

      snowflake id() const;
      snowflake channel_id() const;
      User author() const;
      User user() const;
      std::string content() const;
      std::string text() const;
      std::vector<User> mentions() const;
    };

    inline void from_json(nlohmann::json json, Message& message)
    {
      message = Message(json);
    }
  }
}
