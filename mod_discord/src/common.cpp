#include "common.h"

#include <fstream>

INITIALIZE_EASYLOGGINGPP

namespace ModDiscord
{

  nlohmann::json read_json_file(std::string file)
  {
    std::ifstream ifs(file);
    nlohmann::json json;

    if (ifs.is_open())
    {
      ifs >> json;
    }

    return json;
  }

  void write_json_file(std::string file, nlohmann::json json, bool pretty)
  {
    std::ofstream ofs(file);

    if (ofs.is_open())
    {
      ofs << json.dump(pretty ? 2 : -1);
    }
  }
}