#pragma once

#include <future>
#include <memory>

#include "common.h"
#include "guild.h"
#include "user.h"

namespace ModDiscord
{
  class Gateway;

  class Bot
  {
    Snowflake m_client_id;
    bool m_is_user;
    User m_self;

    std::shared_ptr<Gateway> m_gateway;

    std::vector<Guild> m_guilds;
    std::vector<Channel> m_private_channels;

    std::vector<std::future<void>> m_threads;
    std::mutex m_thread_mutex;

    //  Callbacks for events
    typedef std::function<void(std::shared_ptr<Message>)> OnMessageCallback;
    typedef std::function<void(std::shared_ptr<Emoji>)> OnEmojiChangedCallback;
    OnMessageCallback m_on_message;
    OnEmojiChangedCallback m_on_emoji_created;
    OnEmojiChangedCallback m_on_emoji_deleted;
    OnEmojiChangedCallback m_on_emoji_updated;

    void update_emojis(nlohmann::json data);
  public:
    explicit Bot();

    /** Used to create a Bot. Should be used over constructor.
    
        @param settings The settings with which to load the Bot. Includes token and other miscellaneous data.
     */
    static std::shared_ptr<Bot> create(nlohmann::json settings);

    /** Starts the bot. */
    void run() const;

    /** Get the Bot's user profile.
     
        @return The Bot's user profile.
     */
    std::shared_ptr<User> profile() const;

    /** Get the Bot's invite url.
     
        @return The Bot's invite url.
     */
    std::string invite_url() const;

    /** Called by the Gateway when an event occurs. Should not be called manually. */
    void handle_dispatch(std::string event_name, nlohmann::json data);

    /** Assign a callback for when a message is received. There may only be one callback at a time.
    
        @code
        bot->on_message([](std::shared_ptr<Message> msg){
            if (msg->content() == "Ping!") {
                msg->respond("Pong!");
            }
        });
        @endcode

        @param callback The callback to call when a message is received.
     */
    void on_message(OnMessageCallback callback)
    {
      m_on_message = callback;
    }

    /** Assign a callback for when an emoji is created. There may only be one callback at a time.

        @param callback The callback to call when an emoji is created.
    */
    void on_emoji_created(OnEmojiChangedCallback callback)
    {
      m_on_emoji_created = callback;
    }

    /** Assign a callback for when an emoji is deleted. There may only be one callback at a time.

        @param callback The callback to call when an emoji is deleted.
    */
    void on_emoji_deleted(OnEmojiChangedCallback callback)
    {
      m_on_emoji_deleted = callback;
    }

    /** Assign a callback for when an emoji is updated. There may only be one callback at a time.

        @param callback The callback to call when an emoji is updated.
    */
    void on_emoji_updated(OnEmojiChangedCallback callback)
    {
      m_on_emoji_updated = callback;
    }
  };
}
