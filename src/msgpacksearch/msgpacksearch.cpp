#include "msgpacksearch.h"

#include <cstring>

namespace msgpacksearch
{

Msgpack::Msgpack(const uint8_t *data, size_t length) : data(data), size(length), offset(0) {}

Msgpack::Msgpack(const char *data, size_t length) : Msgpack((uint8_t *)data, length) {}

Msgpack::Msgpack(const std::vector<uint8_t> &data) : Msgpack(data.data(), data.size()) {}

Msgpack::Msgpack(const std::vector<char> &data) : Msgpack((uint8_t *)data.data(), data.size()) {}

FORMAT resolve_format(uint8_t byte)
{
        if (0x00 <= byte && byte <= 0x7f) { // Positive Fixnum
            return FORMAT::POS_FIXINT;
        } else if(0xe0 <= byte && byte <= 0xff) { // Negative Fixnum
           return FORMAT::NEG_FIXINT;
        } else if (0xc4 <= byte && byte <= 0xdf) {
            const uint32_t trail[] = {
                1, // bin     8  0xc4
                2, // bin    16  0xc5
                4, // bin    32  0xc6
                1, // ext     8  0xc7
                2, // ext    16  0xc8
                4, // ext    32  0xc9
                4, // float  32  0xca
                8, // float  64  0xcb
                1, // uint    8  0xcc
                2, // uint   16  0xcd
                4, // uint   32  0xce
                8, // uint   64  0xcf
                1, // int     8  0xd0
                2, // int    16  0xd1
                4, // int    32  0xd2
                8, // int    64  0xd3
                2, // fixext  1  0xd4
                3, // fixext  2  0xd5
                5, // fixext  4  0xd6
                9, // fixext  8  0xd7
                17,// fixext 16  0xd8
                1, // str     8  0xd9
                2, // str    16  0xda
                4, // str    32  0xdb
                2, // array  16  0xdc
                4, // array  32  0xdd
                2, // map    16  0xde
                4, // map    32  0xdf
            };
            return FORMAT(trail[byte - 0xc1]); // verify 
        } else if(0xa0 <= byte && byte <= 0xbf) 
        { 
            // FixStr
            //return FormatInfo(byte & 0b00011111, FORMAT::FIX_STR); // 5 lsb indicate size of fixstr
            return FORMAT::FIX_STR;
        } else if(0x90 <= byte && byte <= 0x9f) 
        { 
            // FixArray
            // return FormatInfo(byte & 0b00001111, FORMAT::FIX_ARRAY); // 4 lsb indicate size of fixarray
            return FORMAT::FIX_ARRAY;
        } else if(0x80 <= byte && byte <= 0x8f) 
        { 
            // FixMap
            //return FormatInfo(byte & 0b00011111, FORMAT::FIX_MAP); // 5 lsb indicate size of fixstr
            return FORMAT::FIX_MAP;
        } else if(byte == 0xc2) 
        { 
            // false
            return FORMAT::FALSE;
        } else if(byte == 0xc3) 
        { 
        // true
            return FORMAT::TRUE;
        } else if(byte == 0xc0) 
        {
         // nil
            return FORMAT::NIL;
        } else 
        {
            std::cerr << "Error Resolving msgpack type: " << std::endl;
            return FORMAT::NIL;
        }
}

uint8_t* Msgpack::find_map_key(const uint8_t *start, const size_t nmb_elements, const std::string &key)
{   
    // start = pointr to start of map
    // nmb_elements = number of elements in map
    // key = key to search for
    // we only care about the top level of the map, so we have to skip non-trivial objects that
    // do not have a matching key

    size_t offset = 0; //offset into the map
    size_t element_counter = 0; // current element being analysed
    uint8_t current_key;

    // terminating condition: we are done when all of the elements have been exhausted.
    while (element_counter < nmb_elements)
    {
        current_key = *(start + offset);


        // currently only handling string keys
        if (current_key == TYPE_MASK::STR8 || (current_key >= 0b10100000 && current_key <= 0b10111111) ||
            current_key  == TYPE_MASK::STR16 || current_key == TYPE_MASK::STR32)
        {
            


        } else
        {
            offset += skip_element(start + offset);
        }
        


    }


    return nullptr;
}


Object Msgpack::get(const std::string &key) 
{
    if (*this->data == TYPE_MASK::MAP16) 
    {
        // map 16 stores a map whose length is upto (2^16)-1 elements
        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+
        // |  0xde  |YYYYYYYY|YYYYYYYY|   N*2 objects   |
        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+

        // index 1 to 2 represents the size of the  Map object which is stored at index 3 to 3 + SIZE.
        
        uint16_t nmb_elements;
        std::memcpy(&nmb_elements, this->data + 1, 2);

        // how to parse a map
        // read a byte, resolve the type, 
        // if type is string, check for equality with 'key'
        // if not equal, skip forward to the next key
        uint8_t* value = find_map_key(this->data + 3, nmb_elements, key);

        if (value)
        {

        }
        else
        {
            return Object(); // return None.
        }
        

        // we will only be handling string keys at the moment


        
       



        

        return Object();

    }
    else if (*this->data == TYPE_MASK::MAP32)
    {
        // map 32 stores a map whose length is upto (2^32)-1 elements
        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
        // |  0xdf  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|   N*2 objects   |
        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
        uint32_t size;
        std::memcpy(&size, this->data + 1, 4);
        

        //find_key(start, end, )
    } else
    {
        
        // return exception
        return Object();
    }
    
    return Object();
}


Object Msgpack::get(const int index)
{
    if (*this->data ==  TYPE_MASK::ARRAY16) 
    {
        // array 16 stores an array whose length is upto (2^16)-1 elements:
        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+
        // |  0xdc  |YYYYYYYY|YYYYYYYY|    N objects    |
        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+

        

    }
    else if (*this->data == TYPE_MASK::ARRAY32)
    {
        // array 32 stores an array whose length is upto (2^32)-1 elements:
        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
        // |  0xdd  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|    N objects    |
        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
        

        //find_key(start, end, )
    } else
    {
        
        // return exception
        return Object();
    }
    
    return Object();
}


} // namespace msgpacksearch