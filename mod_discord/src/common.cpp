#include "common.h"

#include <fstream>

INITIALIZE_EASYLOGGINGPP

namespace ModDiscord
{

  nlohmann::json read_json_file(std::string file)
  {
    std::ifstream ifs(file);
    nlohmann::json json;

    try 
    {
      if (ifs.is_open())
      {
        ifs >> json;
      }
    } 
    catch(std::invalid_argument)
    {
      //  Do nothing and keep the json data empty.
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