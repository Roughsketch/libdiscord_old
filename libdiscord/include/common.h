#pragma once

#include <external/easylogging++.h>
#include <external/json.hpp>

#include "snowflake.h"

namespace Discord
{
  /** Used for methods that take a search method. */
  enum class SearchCriteria
  {
    None,
    Before,
    After,
    Around
  };

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

  /** Allows loading of JSON data into a shared_ptr's underlying type.

  @param json The JSON data to read from.
  @param ptr A shared_ptr that will be initialized with the JSON data.
  */
  template<typename T>
  void from_json(const nlohmann::json& json, std::shared_ptr<T>& ptr)
  {
    ptr = std::make_shared<T>(json);
  }

  /** Allows setting of JSON data from a shared_ptr's underlying type.

  @param json The JSON data to set.
  @param ptr A shared_ptr that will be read into the JSON object.
  */
  template<typename T>
  void to_json(nlohmann::json& json, const std::shared_ptr<T>& ptr)
  {
    json = *ptr;
  }

  /** Allows loading of JSON data into the enum class SearchCriteria.
   
      @param json The JSON data to read from.
      @param search The search variable to set.
   */
  void from_json(const nlohmann::json& json, SearchCriteria& search);
}
