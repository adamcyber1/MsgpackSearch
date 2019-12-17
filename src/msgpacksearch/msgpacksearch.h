#ifndef CMAKE_MSGPACKSEARCH_H
#define CMAKE_MSGPACKSEARCH_H

#include <iostream>
#include <vector>
#include <memory>
#include <variant>
#include <string>

namespace msgpacksearch {

void test() {

  std::cout<<"I must be working...or something. \n";
}

  //TODO : implement the rest of the types
  enum TYPE {
        STR8 = 0xd9,
        STR16 = 0xda,
        STR32 = 0xdb,
        ARRAY16 = 0xdc,
        ARRAY32 = 0xdd,
        MAP16 = 0xde,
        MAP32 = 0xdf
  };

  enum TYPE_MASKS {
    MAP = 0xdf
  };

  typedef std::variant<std::monostate, std::string> obj;

/// @brief Thin wrapper class for reading msgpack data. Read only. Probably will be called Msgpack_View in the near future.
class Msgpack {
  
  public:

  /// Default constructor
  Msgpack();

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
  Msgpack(Msgpack const && other) = delete;

  /// Key based search of an object
  obj get(const std::string &key);

  /// Index based search of an array
  obj get(const int index);

  private:

  /* How should the data ownership be handled....
  * 
  * Case 1: Msgpack doesn't own the underlying data and can only read it. This is unsafe if the data that mspack points to is deallocated....is this acceptable? Can the lifetime
  * of the data be tracked somehow? Msgpack_view ?  USE SHARED_PTR
  * 
  * Case 2: Mspack owns the underlying data, obtained either via move or copy. Read and write is allowed.
  */

  const uint8_t *data;
  const size_t size;
  const size_t offset;
};

}


#endif //CMAKE_MSGPACKSEARCH_H
