#pragma once

#include <cstdint>
#include <variant>

namespace msgpacksearch
{

enum TYPE_MASK {
    MAP16 = 0xde,
    MAP32 = 0xdf,
    ARRAY16 = 0xdc,
    ARRAY32 = 0xdd,
};

/**
 * msgpack_str - represents a string object
 *
 * data -> pointer to the start of the data chunk
 * size -> number of bytes in the data chunk.
 */
struct msgpack_str
{
    msgpack_str(uint32_t size, const char *data) : size(size), data(data) {}
    uint32_t size;
    const char* data;
};

/**
 * msgpack_bin - represents raw binary
 *
 * data -> pointer to the start of the data chunk
 * size -> number of bytes in the data chunk.
 */
struct msgpack_bin
{
    msgpack_bin(uint32_t size, const uint8_t *data) : size(size), data(data) {}
    uint32_t size;
    const uint8_t* data;
};

/**
 * msgpack_map - represents a map object
 *
 * start -> pointer to the start of the map data
 * size -> number of bytes in the map.
 * nmb_elements -> number of elements in the map
 */
struct msgpack_map
{
    msgpack_map(uint32_t nmb_elements, size_t size, const uint8_t *start) : nmb_elements(nmb_elements), size(size), start(start) {}

    uint32_t nmb_elements; // N*2
    size_t size;
    const uint8_t* start;
};

/**
 * msgpack_array - represents an array object
 *
 * start -> pointer to the start of the array data
 * size -> number of bytes in the array.
 * nmb_elements -> number of elements in the array.
 */
struct msgpack_array
{
    msgpack_array(uint32_t nmb_elements, size_t size, const uint8_t *start) : nmb_elements(nmb_elements), size(size), start(start) {}
    uint32_t nmb_elements; // N
    size_t size;
    const uint8_t* start;
};

/**
 * msgpack_ext - represents an extended type object
 *
 * data -> pointer to the start of the ext data
 * size -> number of bytes in the ext.
 * type -> type code for the ext type
 */
struct msgpack_ext {
    msgpack_ext(int8_t type, uint32_t size, const uint8_t *data) : type(type), size(size), data(data) {}
    int8_t type;
    uint32_t size;
    const uint8_t* data;
};

typedef std::variant<std::monostate,
        bool,
        uint64_t,
        int64_t,
        double,
        msgpack_array,
        msgpack_map,
        msgpack_bin,
        msgpack_str,
        msgpack_ext> msgpack_object;
}