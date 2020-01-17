#ifndef CMAKE_MSGPACKSEARCH_H
#define CMAKE_MSGPACKSEARCH_H

#include <iostream>
#include <vector>
#include <memory>
#include <variant>
#include <string>
#include <utility>

#include "types.h"

namespace msgpacksearch {


/// @brief Thin wrapper class for reading msgpack data. Read only. Probably will be called Msgpack_View in the near future.
class Msgpack {
  
  public:

  Msgpack() : data(nullptr), size(0), offset(0) {}
  explicit Msgpack(const std::vector<uint8_t> &data); 
  explicit Msgpack(const std::vector<char> &data);
  explicit Msgpack(const uint8_t *data, size_t length);
  explicit Msgpack(const char *data, size_t length);
  explicit Msgpack(const Msgpack &other) = default;

  Msgpack& operator=(const Msgpack &other) = default;
  Msgpack(Msgpack const && other) = delete;

  /// Key access of a map
  msgpack_object operator[](const std::string &key);

  /// index access of an array
  msgpack_object operator[](const int index);

  /// Key based search of an Object
  msgpack_object get(const std::string &key);

  /// Index based search of an array
  msgpack_object get(const int index);

  /// find the location of key:value in map, returns NULL if not found
  const uint8_t* find_map_key(const msgpack_map &map, const std::string &key);

  /// find the location of key:value in map, return NULL if not found
  const uint8_t* find_map_key(const uint8_t *start, const uint32_t nmb_elements, const std::string &key);

  const uint8_t* find_array_index(const msgpack_array &array, const uint32_t index);
  const uint8_t* find_array_index(const uint8_t *start, const uint32_t nmb_elements, const uint32_t index);
  /// skips an element pointed to by start. Returns the number of bytes skipped
  size_t skip_object(const uint8_t* start);

  /// skips map data, return the number of bytes skipped
  size_t skip_map(const uint8_t* start, const size_t nmb_elements);

  /// skips array data, returns the number of bytes skipped
  size_t skip_array(const uint8_t* start, const size_t nmb_elements);

  // parses an element, returning the number of bytes read and the element
  std::pair<size_t, msgpack_object> parse_data(const uint8_t* start);

  const uint8_t *data;
  const size_t size;
  const size_t offset;
};

}


#endif //CMAKE_MSGPACKSEARCH_H
