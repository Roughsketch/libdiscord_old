#include "bot.h"

#include <spdlog/spdlog.h>

auto Logger = spdlog::stdout_color_mt("main");

void generate_default_settings()
{
  static nlohmann::json default_settings = R"(
  {
    "token": "",
    "client_id": 0,
    "logging": {
      "filename": "",
      "rotate_size": 10485760
    },
    "owner_id": 0,
    "prefix": "",
    "plugin_dir": "",
    "global": {
      
    }
  }
  )"_json;

  ModDiscord::write_json_file("default.json", default_settings, true);
}

bool valid_settings(nlohmann::json settings)
{
  auto valid = true;

  if (!settings.count("token"))
  {
    Logger->error("No token entry found in settings file.");
    valid = false;
  }
  if (!settings.count("client_id"))
  {
    Logger->error("No client_id entry found in settings file.");
    valid = false;
  }
  if (!settings.count("owner_id"))
  {
    Logger->error("No owner_id entry found in settings file.");
    valid = false;
  }
  if (!settings.count("prefix"))
  {
    Logger->warn("No prefix entry found in settings file. It is recommended to set a prefix.");
  }
  if (!settings.count("plugin_dir"))
  {
    Logger->warn("No plugin_dir entry found in settings file. Defaulting to ./plugins");
    settings["plugin_dir"] = "./plugins";
  }

  return valid;
}

int main(int argc, char* argv[])
{
  auto settings = ModDiscord::read_json_file("default.json");

  if (settings.empty())
  {
    Logger->error("No settings file found. Please edit 'default.json' with your bot information.");
    generate_default_settings();
    std::cin.get();
    return EXIT_SUCCESS;
  }

  Logger->trace("Settings loaded.");

  if (!valid_settings(settings))
  {
    Logger->error("One or more errors encountered with settings file. Please update your config and try again.");
    std::cin.get();
    return EXIT_FAILURE;
  }

  auto bot = ModDiscord::Bot::create(settings);

  bot->on_message([bot](std::shared_ptr<ModDiscord::Message> message)
  {
    if (message->content() == ".info")
    {
      message->respond("I am " + bot->profile()->distinct() + " (" + bot->profile()->id().to_string() + ")");
    }
    else if (message->content() == ".emoji")
    {
      auto emojis = message->channel()->guild()->emojis();
      auto response = "There are " + std::to_string(emojis.size()) + " emojis";

      if (emojis.size() > 0)
      {
        response += " including ";
      }

      for (auto& emoji : emojis)
      {
        response += emoji.name() + " ";
      }
      message->respond(response);
    }
    else if (message->content() == ".modify")
    {
      message->channel()->modify([](std::shared_ptr<ModDiscord::Channel> chan)
      {
        chan->set_name("sandcastle");
      });
    }
  });

  bot->run(); //  Start the bot.
  

  std::cout << "Done.";
  std::cin.get();
}
