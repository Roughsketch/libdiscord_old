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
  public:
    explicit Bot();

    static std::shared_ptr<Bot> create(nlohmann::json settings);

    void run() const;

    std::shared_ptr<User> profile() const;
    std::string invite_url() const;
    void handle_dispatch(std::string event_name, nlohmann::json data);

    void update_emojis(nlohmann::json data);

    void on_message(OnMessageCallback callback)
    {
      m_on_message = callback;
    }

    void on_emoji_created(OnEmojiChangedCallback callback)
    {
      m_on_emoji_created = callback;
    }

    void on_emoji_deleted(OnEmojiChangedCallback callback)
    {
      m_on_emoji_deleted = callback;
    }

    void on_emoji_updated(OnEmojiChangedCallback callback)
    {
      m_on_emoji_updated = callback;
    }
  };
}
