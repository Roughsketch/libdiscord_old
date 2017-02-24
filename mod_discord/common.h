#pragma once
#include <cstdint>
#include <fstream>

#include <boost/log/trivial.hpp>

#include "external/json.hpp"

namespace nm = nlohmann;

namespace ModDiscord
{
  typedef uint64_t snowflake;

  inline nm::json read_json_file(std::string file)
  {
    std::ifstream ifs(file);
    nm::json json;

    if (ifs.is_open())
    {
      ifs >> json;
    }

    return json;
  }

  inline void write_json_file(std::string file, nm::json json, bool pretty = false)
  {
    std::ofstream ofs(file);

    if (ofs.is_open())
    {
      ofs << json.dump(pretty ? 2 : -1);
    }
  }
}
