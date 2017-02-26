#pragma once

#include <future>
#include <memory>

#include "guild.h"
#include "user.h"

namespace ModDiscord
{
  class Gateway;

  class Bot
  {
    snowflake m_client_id;
    bool m_is_user;
    User m_self;

    std::shared_ptr<Gateway> m_gateway;

    std::vector<Guild> m_guilds;
    std::vector<Channel> m_private_channels;

    std::vector<std::future<void>> m_threads;
    std::mutex m_thread_mutex;

    //  Callbacks
    std::function<void(std::shared_ptr<Message>)> m_on_message;
  public:
    explicit Bot();

    static std::shared_ptr<Bot> create(nlohmann::json settings);

    void run() const;

    std::shared_ptr<User> profile() const;
    std::string invite_url() const;
    void handle_dispatch(std::string event_name, nlohmann::json data);

    void on_message(std::function<void(std::shared_ptr<Message>)> callback)
    {
      m_on_message = callback;
    }
  };
}
