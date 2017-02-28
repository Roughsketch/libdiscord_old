#pragma once

#include <external/easylogging++.h>
#include <external/json.hpp>

#include "snowflake.h"

namespace ModDiscord
{
  /** Reads JSON data from a file.
   
      @param file The file to read from.
      @return A JSON payload of the file. Payload is empty on failure.
   */
  nlohmann::json read_json_file(std::string file);

  /** Writes JSON data to a file.

      @param file The file to write to.
      @param json The JSON data to write.
      @param pretty Whether output should be pretty (Not one huge line). Defaults to false.
  */
  void write_json_file(std::string file, nlohmann::json json, bool pretty = false);

  /** Set a variable from a json payload, or give it a default value if the key doesn't exist.
   
      @param var The variable to assign the value to.
      @param key The key to grab the new value from.
      @param data The JSON payload to look in.
   */
  template <typename T, typename U>
  void set_from_json(T& var, U key, nlohmann::json data)
  {
    var = (!data.count(key) || data[key].is_null()) ? T() : data[key].template get<T>();
  }

  template<typename T>
  void from_json(const nlohmann::json& json, std::shared_ptr<T>& ptr)
  {
    ptr = std::make_shared<T>(json);
  }
}
