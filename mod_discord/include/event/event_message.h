#pragma once

#include "channel.h"
#include "guild.h"
#include "message.h"
#include "user.h"

namespace ModDiscord
{
  /** An Event wrapper meant to provide convenient methods for handling messages. */
  class MessageEvent
  {
    std::shared_ptr<Message> m_message;
  public:
    explicit MessageEvent(nlohmann::json data)
    {
      m_message = std::make_shared<Message>(data);
    }

    explicit MessageEvent(std::shared_ptr<Message> msg) : m_message(msg) {};

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
  };
}
