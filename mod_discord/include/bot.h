#pragma once

#include <future>
#include <memory>

#include "guild.h"
#include "user.h"

namespace ModDiscord
{
  class Gateway;
  class PluginContainer;

  class Bot : std::enable_shared_from_this<Bot>
  {
    std::shared_ptr<Bot> m_bot;

    std::string m_plugin_dir;
    std::vector<std::shared_ptr<PluginContainer>> m_plugins;
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

    void run() const;
    void load_plugins();

    std::shared_ptr<User> profile() const;
    std::string invite_url() const;
    void handle_dispatch(std::string event_name, nlohmann::json data);
  };
}
