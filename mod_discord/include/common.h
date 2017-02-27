#pragma once
#include <cstdint>
#include <fstream>

#include "external/json.hpp"
#include "snowflake.h"

namespace ModDiscord
{
  inline nlohmann::json read_json_file(std::string file)
  {
    std::ifstream ifs(file);
    nlohmann::json json;

    if (ifs.is_open())
    {
      ifs >> json;
    }

    return json;
  }

  inline void write_json_file(std::string file, nlohmann::json json, bool pretty = false)
  {
    std::ofstream ofs(file);

    if (ofs.is_open())
    {
      ofs << json.dump(pretty ? 2 : -1);
    }
  }

  template <typename T, typename U>
  void set_from_json(T& var, U key, nlohmann::json data)
  {
    var = (!data.count(key) || data[key].is_null()) ? T() : data[key].get<T>();
  }
}
