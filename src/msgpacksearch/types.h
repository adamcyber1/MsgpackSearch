#include <cstdint>
#include <variant>

namespace msgpacksearch
{

enum TYPE_MASK {
    MAP16 = 0xde,
    MAP32 = 0xdf,
    ARRAY16 = 0xdc,
    ARRAY32 = 0xdd,
    STR8   = 0xd9,
    STR16  = 0xda,
    STR32 = 0xdb
};

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