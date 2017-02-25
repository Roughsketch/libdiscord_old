#include "common.h"
#include "bot.h"

void generate_default_settings()
{
  static nm::json default_settings = R"(
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

bool valid_settings(nm::json settings)
{
  auto valid = true;

  if (!settings.count("token"))
  {
    BOOST_LOG_TRIVIAL(error) << "No token entry found in settings file.";
    valid = false;
  }
  if (!settings.count("client_id"))
  {
    BOOST_LOG_TRIVIAL(error) << "No client_id entry found in settings file.";
    valid = false;
  }
  if (!settings.count("owner_id"))
  {
    BOOST_LOG_TRIVIAL(error) << "No owner_id entry found in settings file.";
    valid = false;
  }
  if (!settings.count("prefix"))
  {
    BOOST_LOG_TRIVIAL(warning) << "No prefix entry found in settings file. It is recommended to set a prefix.";
  }
  if (!settings.count("plugin_dir"))
  {
    BOOST_LOG_TRIVIAL(warning) << "No plugin_dir entry found in settings file. Defaulting to ./plugins";
    settings["plugin_dir"] = "./plugins";
  }

  return valid;
}

int main(int argc, char* argv[])
{
  auto settings = ModDiscord::read_json_file("default.json");

  if (settings.empty())
  {
    BOOST_LOG_TRIVIAL(error) << "No settings file found. Please edit 'default.json' with your bot information." << std::endl;
    generate_default_settings();
    std::cin.get();
    return EXIT_SUCCESS;
  }

  BOOST_LOG_TRIVIAL(trace) << "Settings loaded.";

  if (!valid_settings(settings))
  {
    BOOST_LOG_TRIVIAL(error) << "One or more errors encountered with settings file. Please update your config and try again.";
    std::cin.get();
    return EXIT_FAILURE;
  }

  auto bot = ModDiscord::Bot::create(settings);
  bot->run(); //  Start the bot.
  

  std::cout << "Done.";
  std::cin.get();
}
