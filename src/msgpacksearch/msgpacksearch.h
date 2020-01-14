#ifndef CMAKE_MSGPACKSEARCH_H
#define CMAKE_MSGPACKSEARCH_H

#include <iostream>
#include <vector>
#include <memory>
#include <variant>
#include <string>
#include <utility>

#include "object.h"

namespace msgpacksearch {

  //TODO : implement the rest of the types
  /*
  enum TYPE {
        STR8 = 0xd9,
        STR16 = 0xda,
        STR32 = 0xdb,
        ARRAY16 = 0xdc,
        ARRAY32 = 0xdd,
        MAP16 = 0xde,
        MAP32 = 0xdf
  };
  */

  enum FORMAT {
      NIL               = 0x00,
      TRUE              = 0x01,
      FALSE             = 0x02,
      BIN_8             = 0x03,
      BIN_16            = 0x04,
      BIN_32            = 0x05,
      EXT_8             = 0x06,
      EXT_16            = 0x07,
      EXT_32            = 0x09,
      FLOAT_32          = 0x0A,
      FLOAT_64          = 0x0B,
      UINT_8            = 0x0C,
      UINT_16           = 0x0D,
      UINT_32           = 0x0E,
      UINT_64           = 0x0F,
      INT_8             = 0x10,
      INT_16            = 0x11,
      INT_32            = 0x12,
      INT_64            = 0x13,
      FIXEXT_1          = 0x14,
      FIXEXT_2          = 0x15,
      FIXEXT_4          = 0x16,
      FIXEXT_8          = 0x17,
      FIXEXT_16         = 0x18,
      STR_8             = 0X19,
      STR_16            = 0X1A,
      STR_32            = 0X1B,
      ARRAY_16          = 0X1C,
      ARRAY_32          = 0X1D,
      MAP_16            = 0X1E,
      MAP_32            = 0X1F,
      POS_FIXINT        = 0x20,
      NEG_FIXINT        = 0x21,
      FIX_MAP           = 0x22,
      FIX_ARRAY         = 0x23,
      FIX_STR           = 0x24
  };

  enum TYPE_MASK {
    MAP16 = 0xde,
    MAP32 = 0xdf,
    ARRAY16 = 0xdc,
    ARRAY32 = 0xdd,
    STR8   = 0xd9,
    STR16  = 0xda,
    STR32 = 0xdb  
  };

  struct FormatInfo
  {
    FormatInfo() = default;
    FormatInfo(size_t size, FORMAT format) : size(size), format(format) {}

    size_t size; // 
    FORMAT format;
    uint8_t *data; // pointer to where the data starts
  };

  class MsgpackObject
  {
  public:
  MsgpackObject() = default;

  const uint8_t *start;
  FORMAT format;
  };

  typedef std::variant<std::monostate, std::string> Object;

/// @brief Thin wrapper class for reading msgpack data. Read only. Probably will be called Msgpack_View in the near future.
class Msgpack {
  
  public:

  /// Default constructor
  Msgpack() : data(nullptr), size(0), offset(0) {}

  /// Construct from std::vector
  explicit Msgpack(const std::vector<uint8_t> &data); 
  explicit Msgpack(const std::vector<char> &data);

  /// Construct from pointer and size
  explicit Msgpack(const uint8_t *data, size_t length);
  explicit Msgpack(const char *data, size_t length);

  /// Copy constructor
  Msgpack(const Msgpack &other) = default;

  /// Copy Assignment Operator
  Msgpack& operator=(const Msgpack &other) = default;

  /// Move constructor
  /// Msgpack(Msgpack const && other) = delete;

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
