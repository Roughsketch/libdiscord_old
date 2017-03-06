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

  LOG(INFO) << "Settings loaded.";

  if (!valid_settings(settings))
  {
    LOG(ERROR) << "One or more errors encountered with settings file. Please update your config and try again.";
    std::cin.get();
    return EXIT_FAILURE;
  }

  auto bot = Discord::Bot::create(settings);

  bot->add_command("info", [bot](Discord::MessageEvent event)
  {
    event.respond("I am " + bot->profile()->distinct() + " (" + bot->profile()->id().to_string() + ")");
  });

  bot->add_command("emoji", [](Discord::MessageEvent event)
  {      
    auto emojis = event.channel()->guild()->emojis();
    auto response = "There are " + std::to_string(emojis.size()) + " emojis";

    if (emojis.size() > 0)
    {
      response += " including ";
    }

    for (auto& emoji : emojis)
    {
      response += emoji->name() + " ";
    }
    event.respond(response);
  });

  bot->add_command("modify", [](Discord::MessageEvent event)
  {
    try
    {
      event.channel()->modify([](std::shared_ptr<Discord::Channel> chan)
      {
        chan->set_name("sandcastle");
      });
    }
    catch (const Discord::PermissionException&)
    {
      event.respond("I do not have the permissions to modify a channel.");
    }
  });

  bot->add_command("invite", [bot](Discord::MessageEvent event) {
    event.respond("Invite me with this link: " + bot->invite_url());
  });

  bot->add_command("guilds", [bot](Discord::MessageEvent event) {
    std::string response = "I am currently in the following guilds:\n```";

    for (auto& guild : bot->guilds())
    {
      response += guild->name() + ": " + std::to_string(guild->member_count()) + "\n";
    }

    response += "```";

    event.respond(response);
  });


  bot->add_command("mem", [](Discord::MessageEvent event) {
    auto current_mb = std::round(RSS::current() / 1000.0) / 1000.0;
    auto peak_mb = std::round(RSS::peak() / 1000.0) / 1000.0;
    event.respond("```Current memory usage: " + std::to_string(current_mb) + "MB\nPeak memory usage:    " + std::to_string(peak_mb) + "MB```");
  });

  bot->add_command("layout", [](Discord::MessageEvent event)
  {
    std::string response = "```Channel: ";

    response += event.channel()->name() + " (" + event.channel()->id().to_string() + ")\n";
    response += "Guild: " + event.guild()->name() + " (" + event.guild()->id().to_string() + ")\n";
    response += "User: " + event.guild()->get_user(event.author()->id())->distinct() + " (" + event.author()->id().to_string() + ")\n";
    response += "```";

    event.respond(response);
  });

  bot->add_command("test", [](Discord::MessageEvent event)
  {
    event << "Got into test command.";
    event << "Can respond twice.";
  });

  bot->add_command("new", [bot](Discord::MessageEvent event)
  {
    auto channel_name = event.content().substr(event.content().find_first_of(" ") + 1);

    try
    {
      event.guild()->create_text_channel(channel_name);
      event.respond("Created new channel " + channel_name + ".");
    }
    catch (const Discord::PermissionException&)
    {
      event.respond("I do not have the permission to create a channel.");
    }
  });

  bot->add_command("rem", [bot](Discord::MessageEvent event)
  {
    auto channel_name = event.content().substr(event.content().find_first_of(" ") + 1);
    auto channel = event.guild()->find_channel(channel_name);

    if (channel)
    {
      try
      {
        channel->remove();
        event.respond("Removed " + channel_name + ".");
      }
      catch (const Discord::PermissionException&)
      {
        event.respond("I do not have the permission to remove a channel.");
      }
    }
    else
    {
      event.respond("Could not find channel with name " + channel_name + ".");
    }
  });

  bot->add_command("help", [bot](Discord::MessageEvent event)
  {
    event << "List of commands:```"
          << "help  : This command.\n"
          << "info  : Info placeholder.\n"
          << "guilds: A list of guilds this bot is currently in.\n"
          << "new   : Create a new channel.\n"
          << "rem   : Remove a channel.\n"
          << "mem   : Get current a peak memory of this bot process.\n"
          << "layout: Test if user, channel, and guild information can be retrieved correctly.\n"
          << "```";

  });

  bot->add_command("prune", [](Discord::MessageEvent event)
  {
    auto word = event.content().substr(event.content().find_first_of(" ") + 1);
    int amount = std::stoul(word);

    try
    {
      event.channel()->prune(amount);
    }
    catch (const Discord::DiscordException& e)
    {
      event.respond(e.what());
    }
  });

  bot->add_command("embed", [](Discord::MessageEvent event)
  {
    Discord::Embed embed;

    embed.set_title("Creating an embed.");
    embed.set_description("This is how you create an embed object.");
    embed.set_author("Roughsketch",
                      "https://github.com/Roughsketch",
                      "https://avatars2.githubusercontent.com/u/1938661?v=3&s=40");
    embed.add_field("Field Example 1", "This is an inline field", true);
    embed.add_field("Field Example 2", "This is also an inline field", true);
    embed.add_field("Field Example 3", "This isn't an inline field");
    embed.set_footer("This is a footer with just text.");

    event.channel()->send_embed(embed);
  });

  bot->run(); //  Start the bot.
}
