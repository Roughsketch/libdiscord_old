#pragma once

#include <memory>

#include "common.h"
#include "gateway.h"
#include "guild.h"

namespace ModDiscord
{
  class Gateway;

  class Bot
  {
    std::string m_token;
    snowflake m_client_id;
    bool m_is_user;
    User m_self;

    std::shared_ptr<Gateway> m_gateway;

    std::vector<Guild> m_guilds;
    std::vector<Channel> m_private_channels;

    std::vector<std::future<void>> m_threads;
    std::mutex m_thread_mutex;
  public:
    explicit Bot();

    static std::shared_ptr<Bot> create(nlohmann::json settings);

    std::shared_ptr<Gateway> gateway() const;
    void run() const;

    std::string invite_url() const;
    void handle_dispatch(std::string event_name, nlohmann::json data);
  };
}
