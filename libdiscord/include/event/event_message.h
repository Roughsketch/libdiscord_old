#pragma once

#include "common.h"
#include "identifiable.h"

namespace Discord
{
  class Channel;
  class Guild;
  class Message;
  class User;

  template <typename T>
  class Respondable
  {
    T& m_owner;
    std::stringstream m_stream;
  public:
    explicit Respondable(T& owner) : m_owner(owner) {}

    Respondable(Respondable<T>& other)
    {
      m_owner = other->m_owner;
      m_stream << other->m_stream.str();
    }

    ~Respondable()
    {
      m_owner << m_stream.str();
    }

    template <typename U>
    Respondable& operator<<(U& obj)
    {
      m_stream << obj;
      return *this;
    }
  };

  /** An Event wrapper meant to provide convenient methods for handling messages. */
  class MessageEvent
  {
    std::shared_ptr<Message> m_message;
  public:
    explicit MessageEvent(nlohmann::json data);
    explicit MessageEvent(std::shared_ptr<Message> msg) : m_message(msg) {};

    void operator<<(std::string& message) const
    {
      if (!message.empty())
      {
        respond(message);
      }
    }

    /** Get the user who posted this message.
     
        @return A shared pointer of the author of this message.
     */
    std::shared_ptr<User> author() const;

    /** Get the user who posted this message. Alias of author.
     
        @return A shared pointer of the author of this message.
     */
    std::shared_ptr<User> user() const;

    /** Get the content of the message.
     
        @return The content of the message.
     */
    std::string content() const;

    /** Get the channel this message was posted in.
     
        @return The channel this message was posted in.
     */
    std::shared_ptr<Channel> channel() const;

    /** Get the guild this message was posted in.
     
        @return The guild this message was posted in.
     */
    std::shared_ptr<Guild> guild() const;

    /** Get the message that was sent.
     
        @return The message that was sent.
     */
    std::shared_ptr<Message> message() const;

    /** Whether or not this message was sent from a bot.
     
        @return true if this message was sent from a bot.
     */
    bool from_bot() const;

    /** Sends a message to the channel which this message was posted in.     
      
        @return The message that was sent.
     */
    std::shared_ptr<Message> respond(std::string content, bool tts = false) const;

    /** Allow using operator << to respond to an event.
     
        @code
        event->respond() << "This is a response! " << "And more!";
        @endcode
     
        @return An object you can use like a stream.
     */
    Respondable<MessageEvent> respond();
  };

  class MessageDeletedEvent : public Identifiable
  {
    Snowflake m_channel_id;
  public:
    explicit MessageDeletedEvent(Snowflake id, Snowflake channel_id);
    explicit MessageDeletedEvent(nlohmann::json data);

    /** Get the channel this message was deleted from.

        @return The channel this message was deleted from.
    */
    std::shared_ptr<Channel> channel() const;

    /** Get the guild this message was deleted from.

        @return The guild this message was deleted from.
    */
    std::shared_ptr<Guild> guild() const;

    /** Sends a message to the channel where this message was deleted.

        @return The message that was sent.
    */
    std::shared_ptr<Message> respond(std::string content, bool tts = false) const;
  };

  class TypingEvent
  {
    Snowflake m_channel_id;
    Snowflake m_user_id;
    uint32_t m_timestamp;
  public:
    TypingEvent();
    explicit TypingEvent(nlohmann::json data);

    /** Get the user that initiated this typing event.
     
        @return The user who started typing.
     */
    std::shared_ptr<User> author() const;

    /** Get the user that initiated this typing event. Alias of author.

        @return The user who started typing.
    */
    std::shared_ptr<User> user() const;

    /** Get the channel that this event was raised from.

        @return The channel where this typing started.
    */
    std::shared_ptr<Channel> channel() const;

    /** Get the guild that this typing event was raised from.

      @return The guild where this typing started.
    */
    std::shared_ptr<Guild> guild() const;
  };
}
