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

    Msgpack() : _data(nullptr), _size(0), _offset(0) {}
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
    std::string_view get_sv(const std::string &key);
    int get_int(const std::string &key);
    bool get_bool(const std::string &key);
    msgpack_map get_map(const std::string &key);
    msgpack_array get_array(const std::string &key);
    msgpack_bin get_bin(const std::string &key);
    msgpack_ext get_ext(const std::string &key);


    /// Index based search of an array
    msgpack_object get(const int index);
    std::string_view get_sv(const int index);
    int get_int(const int index);
    bool get_bool(const int index);
    msgpack_map get_map(const int index);
    msgpack_array get_array(const int index);
    msgpack_bin get_bin(const int index);
    msgpack_ext get_ext(const int index);

    /**
    * Finds the location of a key in a given map
    *
    * @param[in] map map to traverse.
    * @param[in] key key to search for.
    * @return The location of the value in the key:value pair, or NULL if not found.
    */
    const uint8_t* find_map_key(const msgpack_map &map, const std::string &key);

    /**
    * Finds the location of a key in a given map
    *
    * @param[in] start pointer to the start of the map to traverse.
    * @param[in] nmb_elements number of elements in the map to traverse.
    * @param[in] key key to search for.
    * @return The location of the value in the key:value pair, or NULL if not found.
    */
    const uint8_t* find_map_key(const uint8_t *start, const uint32_t nmb_elements, const std::string &key);

    /**
    * Finds the location of an index in an array
    *
    * @param[in] array array to traverse.
    * @param[in] index index to search for.
    * @return The location of the value @ index, or NULL if not found.
    */
    const uint8_t* find_array_index(const msgpack_array &array, const uint32_t index);

    /**
    * Finds the location of an index in an array
    *
    * @param[in] start points to the start of the array to traverse.
    * @param[in] nmb_elements number of elements in the array to traverse
    * @param[in] index index to search for.
    * @return The location of the value @ index, or NULL if not found.
    */
    const uint8_t* find_array_index(const uint8_t *start, const uint32_t nmb_elements, const uint32_t index);

    /**
    * Skips an object in the msgpack blob
    * @param[in] start points at the object to skip
    * @return Number of bytes skipped
    */
    size_t skip_object(const uint8_t* start);

    /**
    * Skips a map in the msgpack blob
    * @param[in] start points at the map to skip
    * @param[in] nmb_elements number of elements in the map
    * @return Number of bytes skipped
    */
    size_t skip_map(const uint8_t* start, const size_t nmb_elements);

    /**
    * Skips an array in the msgpack blob
    * @param[in] start points at the array to skip
    * @param[in] nmb_elements number of elements in the array
    * @return Number of bytes skipped
    */
    size_t skip_array(const uint8_t* start, const size_t nmb_elements);

    /**
    * Parses an object in the msgpack blob
    * @param[in] start points at the start of the object to parse
    * @return Number of bytes parsed, and the msgpack object
    */
    std::pair<size_t, msgpack_object> parse_data(const uint8_t* start);

    /**
    * Getter for _data
    * @return const pointer to the raw data
    */
    const uint8_t* data();

    /**
    * Getter for _size
    * @return size of the data in bytes
    */
    const size_t size();

    /**
    * Getter for _offset
    * @return offset into the data
    */
    const size_t offset();

private:
    const uint8_t *_data;
    const size_t _size;
    const size_t _offset;
};

}


#endif //CMAKE_MSGPACKSEARCH_H
