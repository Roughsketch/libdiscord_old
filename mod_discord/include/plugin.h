#pragma once
#include <string>
#include "bot.h"
#include "message.h"

namespace ModDiscord
{
  class Bot;

  enum PluginResponse : uint8_t
  {
    Pass,
    Handled
  };

  class Plugin
  {
  public:
    virtual std::string name() = 0;
    virtual std::string author() = 0;
    virtual std::string version() = 0;

    virtual PluginResponse OnMessage(std::shared_ptr<const Bot> bot, std::shared_ptr<Message> message)
    {
      return PluginResponse::Pass;
    }
  };
}
