#include "bot.h"

#include "api.h"
#include "channel.h"
#include "message.h"

namespace ModDiscord
{
  Bot::Bot(nm::json settings)
  {
    set_from_json(m_token, "token", settings);
    set_from_json(m_client_id, "client_id", settings);
    set_from_json(m_is_user, "user_account", settings);

    if (!m_is_user)
    {
      m_token = "Bot " + m_token;
    }

    ModDiscord::API::set_token(m_token);

    m_gateway = std::make_shared<Gateway>(m_token);
  }

  std::shared_ptr<Gateway> Bot::gateway() const
  { 
    return m_gateway;
  }

  void Bot::run() const
  {
    m_gateway->start();
  }

  std::string Bot::invite_url() const
  {
    return "https://discordapp.com/oauth2/authorize?client_id=" + std::to_string(m_client_id) + "&scope=bot";
  }

  void Bot::handle_dispatch(std::string event_name, nlohmann::json data)
  {
    BOOST_LOG_TRIVIAL(info) << "Bot.handle_dispatch entered with " << event_name.c_str() << ".";

    if (event_name == "READY")
    {
      set_from_json(m_self, "user", data);
      set_from_json(m_private_channels, "private_channels", data);
    }
    else if (event_name == "GUILD_CREATE")
    {
      m_guilds.emplace_back(Guild(data));
    }
    else if (event_name == "MESSAGE_CREATE")
    {
      m_threads.push_back(std::async([&]() {
        auto msg = std::make_shared<Message>(data);
        BOOST_LOG_TRIVIAL(info) << "Got message: " << msg->content();

        if (msg->content() == ".guilds")
        {
          BOOST_LOG_TRIVIAL(info) << "Currently have " << m_guilds.size() << " guilds.";
          auto chan = API::Channel::get_channel(msg->channel_id());
          chan->send_message("This message is sent directly from a channel object.");
          chan->send_temp_message("This is a temporary message.", 5);
        }
        else if (msg->content() == ".info")
        {
          msg->respond("I am " + m_self.distinct() + " (" + std::to_string(m_self.id()) + ")");
        }
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
