#include "bot.h"

namespace ModDiscord
{
  Bot::Bot(nm::json settings)
  {
    m_token = settings["token"].get<std::string>();
    m_client_id = settings["client_id"];

    m_gateway = std::make_shared<Gateway>(m_token);
  }

  std::string Bot::invite_url() const
  {
    return "https://discordapp.com/oauth2/authorize?client_id=" + std::to_string(m_client_id) + "&scope=bot";
  }
}