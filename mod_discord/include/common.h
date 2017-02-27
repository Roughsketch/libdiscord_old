#pragma once

#include <spdlog/spdlog.h>
#include <external/json.hpp>

#include "snowflake.h"

namespace ModDiscord
{
  static auto Logger = spdlog::stdout_color_mt("console");

  nlohmann::json read_json_file(std::string file);
  void write_json_file(std::string file, nlohmann::json json, bool pretty = false);

  template <typename T, typename U>
  void set_from_json(T& var, U key, nlohmann::json data)
  {
    var = (!data.count(key) || data[key].is_null()) ? T() : data[key].get<T>();
  }
}
