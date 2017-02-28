#include "mod_discord.h"

#include <external/easylogging++.h>

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
    LOG(ERROR) << "No token entry found in settings file.";
    valid = false;
  }
  if (!settings.count("client_id"))
  {
    LOG(ERROR) << "No client_id entry found in settings file.";
    valid = false;
  }
  if (!settings.count("owner_id"))
  {
    LOG(ERROR) << "No owner_id entry found in settings file.";
    valid = false;
  }
  if (!settings.count("prefix"))
  {
    LOG(WARNING) << "No prefix entry found in settings file. It is recommended to set a prefix.";
  }
  if (!settings.count("plugin_dir"))
  {
    LOG(WARNING) << "No plugin_dir entry found in settings file. Defaulting to ./plugins";
    settings["plugin_dir"] = "./plugins";
  }

  return valid;
}

int main(int argc, char* argv[])
{
  auto settings = ModDiscord::read_json_file("default.json");

  if (settings.empty())
  {
    LOG(ERROR) << "No settings file found. Please edit 'default.json' with your bot information.";
    generate_default_settings();
    std::cin.get();
    return EXIT_SUCCESS;
  }

  LOG(TRACE) << "Settings loaded.";

  if (!valid_settings(settings))
  {
    LOG(ERROR) << "One or more errors encountered with settings file. Please update your config and try again.";
    std::cin.get();
    return EXIT_FAILURE;
  }

  auto bot = ModDiscord::Bot::create(settings);

  bot->on_message([bot](std::shared_ptr<ModDiscord::MessageEvent> event)
  {
    if (event->content() == ".info")
    {
      event->respond("I am " + bot->profile()->distinct() + " (" + bot->profile()->id().to_string() + ")");
    }
    else if (event->content() == ".emoji")
    {
      auto emojis = event->channel()->guild()->emojis();
      auto response = "There are " + std::to_string(emojis.size()) + " emojis";

      if (emojis.size() > 0)
      {
        response += " including ";
      }

      for (auto& emoji : emojis)
      {
        response += emoji->name() + " ";
      }
      event->respond(response);
    }
    else if (event->content() == ".modify")
    {
      event->channel()->modify([](std::shared_ptr<ModDiscord::Channel> chan)
      {
        chan->set_name("sandcastle");
      });
    }
    else if (event->content() == ".invite")
    {
      event->respond("Invite me with this link: " + bot->invite_url());
    }
  });

  bot->run(); //  Start the bot.
  
  std::cout << "Done.";
  std::cin.get();
}
