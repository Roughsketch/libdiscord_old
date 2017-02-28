#include "event/event_message.h"

#include "api/api_channel.h"
#include "channel.h"
#include "guild.h"
#include "message.h"
#include "user.h"

namespace ModDiscord
{
  MessageEvent::MessageEvent(nlohmann::json data)
  {
    m_message = std::make_shared<Message>(data);
  }

  std::shared_ptr<User> MessageEvent::author() const
  {
    return m_message->author();
  }

  std::shared_ptr<User> MessageEvent::user() const
  {
    return author();
  }

  std::string MessageEvent::content() const
  {
    return m_message->content();
  }

  std::shared_ptr<Channel> MessageEvent::channel() const
  {
    return m_message->channel();
  }

  std::shared_ptr<Guild> MessageEvent::guild() const
  {
    return m_message->channel()->guild();
  }

  std::shared_ptr<Message> MessageEvent::message() const
  {
    return m_message;
  }

  bool MessageEvent::from_bot() const
  {
    return m_message->author()->is_bot();
  }

  std::shared_ptr<Message> MessageEvent::respond(std::string content, bool tts) const
  {
    return m_message->respond(content, tts);
  }

  MessageDeletedEvent::MessageDeletedEvent(Snowflake id, Snowflake channel_id) : Identifiable(id)
  {
    m_channel_id = channel_id;
  }

  MessageDeletedEvent::MessageDeletedEvent(nlohmann::json data)
  {
    set_id_from_json("id", data);
    set_from_json(m_channel_id, "channel_id", data);
  }

  std::shared_ptr<Channel> MessageDeletedEvent::channel() const
  {
    return ModDiscord::API::Channel::get_channel(m_channel_id);
  }

  std::shared_ptr<Guild> MessageDeletedEvent::guild() const
  {
    return channel()->guild();
  }

  std::shared_ptr<Message> MessageDeletedEvent::respond(std::string content, bool tts) const
  {
    channel()->send_message(content, tts);
  }
}
