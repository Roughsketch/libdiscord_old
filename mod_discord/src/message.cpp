#include "api.h"
#include "message.h"

namespace ModDiscord
{
  Message::Message()
  {
    m_channel_id = 0;
    m_tts = false;
    m_mention_everyone = false;
    m_nonce = 0;
    m_pinned = false;
  }

  Message::Message(nlohmann::json data)
  {
    set_from_json(m_id, "id", data);
    set_from_json(m_channel_id, "channel_id", data);
    set_from_json(m_author, "author", data);
    set_from_json(m_content, "content", data);
    set_from_json(m_timestamp, "timestamp", data);
    set_from_json(m_edited_timestamp, "edited_timestamp", data);
    set_from_json(m_tts, "tts", data);
    set_from_json(m_mention_everyone, "mention_everyone", data);
    set_from_json(m_mentions, "mentions", data);
    set_from_json(m_mention_roles, "mention_roles", data);
    // set_from_json(m_attachments, "attachments", data);
    // set_from_json(m_embeds, "embeds", data);
    // set_from_json(m_reactions, "reactions", data);
    set_from_json(m_nonce, "nonce", data);
    set_from_json(m_pinned, "pinned", data);
    set_from_json(m_webhook_id, "webhook_id", data);
  }

  snowflake Message::channel_id() const
  {
    return m_channel_id;
  }
  User Message::author() const
  {
    return m_author;
  }

  User Message::user() const
  {
    return author();
  }

  std::string Message::content() const
  {
    return m_content;
  }

  std::string Message::text() const
  {
    return content();
  }

  std::vector<User> Message::mentions() const
  {
    return m_mentions;
  }

  std::shared_ptr<Channel> Message::channel() const
  {
    return ModDiscord::API::Channel::get_channel(channel_id());
  }

  bool Message::remove() const
  {
    return ModDiscord::API::Channel::delete_message(channel_id(), id());
  }

  std::shared_ptr<Message> Message::respond(std::string message, bool tts) const
  {
    return ModDiscord::API::Channel::create_message(channel_id(), message, tts);
  }
}