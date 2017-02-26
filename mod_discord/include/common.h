#pragma once
#include <cstdint>
#include <fstream>

#include "external/json.hpp"

namespace ModDiscord
{
  typedef uint64_t snowflake;

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

  //  Specialization for snowflake because it's sent as a string, but is really a uint64_t
  template <typename U>
  void set_from_json(snowflake& var, U key, nlohmann::json data)
  {
    var = (!data.count(key) || data[key].is_null()) ? snowflake() : std::stoull(data[key].get<std::string>());
  }

  //  Specialization for vector snowflake
  template <typename U>
  void set_from_json(std::vector<snowflake>& var, U key, nlohmann::json data)
  {
    if (!data.count(key) || data[key].is_null())
    {
      var = std::vector<snowflake>();
    }
    else
    {
      auto ids = data[key].get<std::vector<std::string>>();
      std::transform(std::begin(ids), std::end(ids), std::back_inserter(var), 
        [](std::string a) -> snowflake {
          return std::stoull(a); 
        });
    }
  }
}
