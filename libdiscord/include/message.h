#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class Channel;
  class Role;
  class User;

  class Message : public Identifiable
  {
    Snowflake m_channel_id;
    std::shared_ptr<User> m_author;
    std::string m_content;
    std::string m_timestamp;
    std::string m_edited_timestamp;
    bool m_tts;
    bool m_mention_everyone;
    std::vector<std::shared_ptr<User>> m_mentions;
    std::vector<std::shared_ptr<Role>> m_mention_roles;
    // std::vector<Attachment> m_attachments;
    // std::vector<Embed> m_embeds;
    // std::vector<Reaction> m_reactions;
    Snowflake m_nonce;
    bool m_pinned;
    std::string m_webhook_id;
  public:
    Message();
    explicit Message(nlohmann::json data);

    Snowflake channel_id() const;
    std::shared_ptr<User> author() const;
    std::shared_ptr<User> user() const;
    std::string content() const;
    std::string text() const;
    std::vector<std::shared_ptr<User>> mentions() const;

    std::shared_ptr<Channel> channel() const;
    bool remove() const;
    std::shared_ptr<Message> respond(std::string message, bool tts = false) const;
  };

  inline void from_json(nlohmann::json json, Message& message)
  {
    message = Message(json);
  }
}
