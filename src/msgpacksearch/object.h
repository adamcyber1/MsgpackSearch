#include <cstdint>
#include <variant>

namespace msgpacksearch
{

 enum msgpack_object_type{
    MSGPACK_OBJECT_NIL                  = 0x00,
    MSGPACK_OBJECT_BOOLEAN              = 0x01,
    MSGPACK_OBJECT_POSITIVE_INTEGER     = 0x02,
    MSGPACK_OBJECT_NEGATIVE_INTEGER     = 0x03,
    MSGPACK_OBJECT_FLOAT32              = 0x0a,
    MSGPACK_OBJECT_FLOAT64              = 0x04,
    MSGPACK_OBJECT_FLOAT                = 0x04,
    MSGPACK_OBJECT_STR                  = 0x05,
    MSGPACK_OBJECT_ARRAY                = 0x06,
    MSGPACK_OBJECT_MAP                  = 0x07,
    MSGPACK_OBJECT_BIN                  = 0x08,
    MSGPACK_OBJECT_EXT                  = 0x09
};

// basic types

struct msgpack_str
{
    msgpack_str(uint32_t size, const char *data) : size(size), data(data) {}
    uint32_t size;
    const char* data;
};

struct msgpack_bin
{
    msgpack_bin(uint32_t size, const uint8_t *data) : size(size), data(data) {}
    uint32_t size;
    const uint8_t* data;
};

struct msgpack_map
{
    msgpack_map(uint32_t nmb_elements, size_t size, const uint8_t *start) : nmb_elements(nmb_elements), size(size), start(start) {}

    uint32_t nmb_elements; // N * 2
    size_t size; // size in bytes
    const uint8_t* start; // points to the first byte of the elements
};

struct msgpack_array
{
    msgpack_array(uint32_t size, const uint8_t *data) : size(size), data(data) {}
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
             msgpack_str> msgpack_object;

}