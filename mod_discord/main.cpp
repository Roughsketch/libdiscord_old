#include "common.h"
#include "gateway.h"

void generate_default_settings()
{
  static nm::json default_settings = R"(
  {
    "token": "YOUR_BOT_TOKEN",
    "client_id": "YOUR_CLIENT_ID",
    "prefix": "YOUR_PREFIX",
    "plugin_dir": "YOUR_PLUGIN_DIRECTORY",
    "global": {
      
    }
  }
  )"_json;

  ModDiscord::write_json_file("default.json", default_settings, true);
}

int main(int argc, char* argv[])
{
  auto settings = ModDiscord::read_json_file("default.json");

  if (settings.empty())
  {
    std::cout << "No settings file found. Please edit 'default.json' with your bot information." << std::endl;
    generate_default_settings();
  }
  else
  {
    std::cout << "Settings file loaded." << std::endl;
  }

  settings["token"] = "This.isa.bot.token.$";

  std::cout << settings.dump(2) << std::endl;

  std::cout << "URL:" << ModDiscord::API::get_wss_url();

  std::cin.get();
}
