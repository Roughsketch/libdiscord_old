#include <discord.h>

#define ELPP_THREAD_SAFE
#define ELPP_FORCE_USE_STD_THREAD
#include "external/easylogging++.h"
#include "external/getRSS.h"

#include <iostream>
#include <memory>

void generate_default_settings()
{
  static nlohmann::json default_settings = R"(
  {
    "token": "",
    "client_id": "0",
    "logging": {
      "filename": "",
      "rotate_size": 10485760
    },
    "owner_id": "0",
    "prefix": "",
    "plugin_dir": "",
    "global": {
      
    }
  }
  )"_json;

  Discord::write_json_file("default.json", default_settings, true);
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
  auto settings = Discord::read_json_file("default.json");

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

  auto bot = Discord::Bot::create(settings);

  bot->on_message([bot](std::shared_ptr<Discord::MessageEvent> event)
  {
    LOG(INFO) << "Got into OnMessage handler with message " << event->content();
    if (event->content() == "md.info")
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
    else if (event->content() == "md.modify")
    {
      event->channel()->modify([](std::shared_ptr<Discord::Channel> chan)
      {
        chan->set_name("sandcastle");
      });
    }
    else if (event->content() == "md.invite")
    {
      event->respond("Invite me with this link: " + bot->invite_url());
    }
    else if (event->content() == "md.guilds")
    {
      std::string response = "I am currently in the following guilds:\n```";

      for (auto& guild : bot->guilds())
      {
        response += guild->name() + ": " + std::to_string(guild->member_count()) + "\n";
      }

      response += "```";

      event->respond(response);
    }
    else if (event->content() == "md.mem")
    {
      auto current_mb = std::round(RSS::current() / 1000.0) / 1000.0;
      auto peak_mb = std::round(RSS::peak() / 1000.0) / 1000.0;
      event->respond("```Current memory usage: " + std::to_string(current_mb) + "MB\nPeak memory usage:    " + std::to_string(peak_mb) + "MB```");
    }
    else if (event->content() == "md.layout")
    {
      std::string response = "```Channel: ";

      response += event->channel()->name() + " (" + event->channel()->id().to_string() + ")\n";
      response += "Guild: " + event->guild()->name() + " (" + event->guild()->id().to_string() + ")\n";
      response += "User: " + event->guild()->get_user(event->author()->id())->distinct() + " (" + event->author()->id().to_string() + ")\n";
      response += "```";

      event->respond(response);
    }
  });

  bot->add_command("test", [](std::shared_ptr<Discord::MessageEvent> event)
  {
    event->respond("Got into test command.");
  });

  bot->run(); //  Start the bot.
  
  std::cout << "Done.";
  std::cin.get();
}
