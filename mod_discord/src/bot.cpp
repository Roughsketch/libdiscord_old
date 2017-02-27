#include "bot.h"

#include "api.h"
#include "common.h"
#include "gateway.h"
#include "plugin_container.h"

#include <boost/log/trivial.hpp>

namespace ModDiscord
{
  Bot::Bot()
  {
    m_is_user = false;
  }

  std::shared_ptr<Bot> Bot::create(nlohmann::json settings)
  {
    auto bot = std::make_shared<Bot>();

    set_from_json(bot->m_client_id, "client_id", settings);
    set_from_json(bot->m_is_user, "user_account", settings);

    std::string token;
    set_from_json(token, "token", settings);

    if (!bot->m_is_user)
    {
      token = "Bot " + token;
    }

    ModDiscord::API::set_token(token);

    bot->m_gateway = std::make_shared<Gateway>(token);
    bot->m_gateway->set_bot(bot); //  Let the gateway know about the bot so it can send events.

    return bot;
  }

  void Bot::run() const
  {
    m_gateway->start();
  }

  std::shared_ptr<User> Bot::profile() const
  {
    return std::make_shared<User>(m_self);
  }

  std::string Bot::invite_url() const
  {
    return "https://discordapp.com/oauth2/authorize?client_id=" + m_client_id.to_string() + "&scope=bot";
  }

  void Bot::handle_dispatch(std::string event_name, nlohmann::json data)
  {
    BOOST_LOG_TRIVIAL(info) << "Bot.handle_dispatch entered with " << event_name.c_str() << ".";

    if (event_name == "READY")
    {
      set_from_json(m_self, "user", data);
      set_from_json(m_private_channels, "private_channels", data);
    }
    else if (event_name == "CHANNEL_CREATE" || event_name == "CHANNEL_UPDATE")
    {
      ModDiscord::API::Channel::update_cache(std::make_shared<Channel>(data));
    }
    else if (event_name == "CHANNEL_DELETE")
    {
      ModDiscord::API::Channel::remove_cache(std::make_shared<Channel>(data));
    }
    else if (event_name == "GUILD_CREATE" || event_name == "GUILD_UPDATE")
    {
      ModDiscord::API::Guild::update_cache(std::make_shared<Guild>(data));
    }
    else if (event_name == "GUILD_DELETE")
    {
      Snowflake id;
      set_from_json(id, "id", data);

      if (data.count("unavailable"))
      {
        //  The guild is just unavailable, mark it as such.
        ModDiscord::API::Guild::mark_unavailable(id);
      }
      else
      {
        //  The user was removed from the guild, remove it from our cache.
        ModDiscord::API::Guild::remove_cache(id);
      }
    }
    else if (event_name == "GUILD_BAN_ADD")
    {
      auto user = std::make_shared<User>(data);
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"].get<Snowflake>());
      BOOST_LOG_TRIVIAL(info) << "User " << user->distinct() << " has been banned from " << guild->name();
    }
    else if (event_name == "GUILD_BAN_REMOVE")
    {
      auto user = std::make_shared<User>(data);
      auto guild = ModDiscord::API::Guild::get_guild(data["guild_id"].get<Snowflake>());
      BOOST_LOG_TRIVIAL(info) << "User " << user->distinct() << " has been unbanned from " << guild->name();
    }
    else if (event_name == "GUILD_EMOJIS_UPDATE")
    {
      
    }
    else if (event_name == "MESSAGE_CREATE")
    {
      m_threads.push_back(std::async([&]() {
        auto msg = std::make_shared<Message>(data);
        BOOST_LOG_TRIVIAL(trace) << "Got message: " << msg->content();
        m_on_message(msg);
      }));
    }

    //  If we can lock the thread array, then do so.
    if (m_thread_mutex.try_lock())
    {
      std::lock_guard<std::mutex> lock(m_thread_mutex, std::adopt_lock);

      //  Remove all futures from the array which have finished.
      m_threads.erase(
        std::remove_if(std::begin(m_threads), std::end(m_threads),
          [](const std::future<void>& f) {
            using namespace std::chrono;
            return f.wait_until(system_clock::now() + 1ms) == std::future_status::ready;
      }), std::end(m_threads));
    }
  }
}
