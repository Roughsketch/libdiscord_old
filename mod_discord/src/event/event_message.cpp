#include "event/event_message.h"

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
}
