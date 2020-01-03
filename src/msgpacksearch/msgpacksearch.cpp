#include "msgpacksearch.h"

#include <cstring>
#include <iostream>
#include <bits/byteswap.h>

namespace msgpacksearch
{

Msgpack::Msgpack(const uint8_t *data, size_t length) : data(data), size(length), offset(0) {}

Msgpack::Msgpack(const char *data, size_t length) : Msgpack((uint8_t *)data, length) {}

Msgpack::Msgpack(const std::vector<uint8_t> &data) : Msgpack(data.data(), data.size()) {}

Msgpack::Msgpack(const std::vector<char> &data) : Msgpack((uint8_t *)data.data(), data.size()) {}

std::pair<size_t, msgpack_object> Msgpack::parse_data(const uint8_t* start)
{
     size_t bytes_read = 0;
     size_t offset = 0;

     switch (*start)
     {
         case 0x00 ... 0x7f: // positive fixnum
         {
            //  positive fixnum stores 7-bit positive integer
            // +--------+
            // |0XXXXXXX|
            // +--------+
            return std::make_pair<size_t, msgpack_object>(1, static_cast<uint64_t>(*start));
         }
         case 0xe0 ... 0xff: // negative fixnum
         {
            // negative fixnum stores 5-bit negative integer
            // +--------+
            // |111YYYYY|
            // +--------+
            // TODO convert 5-bit negative integer to negative int64_t

            return std::make_pair<size_t, msgpack_object>(1, static_cast<int64_t>(*start));

         }
         case 0xc0 ... 0xdf: // variable length types
         {
              switch(*start) 
              {
                    case 0xc0:
                    {
                        return std::make_pair<size_t, msgpack_object>(1, std::monostate());
                    }
                    case 0xc2:  // false
                    {
                        return std::make_pair<size_t, msgpack_object>(1, false);
                    }
                    case 0xc3:  // true
                    {
                        return std::make_pair<size_t, msgpack_object>(1, true);
                    }
                    case 0xc4: // bin 8
                    {
                        // +--------+--------+========+
                        // |  0xc4  |XXXXXXXX|  data  |
                        // +--------+--------+========+

                        uint8_t bin8_size;
                        std::memcpy(&bin8_size, start + 1, sizeof(bin8_size));

                        return std::make_pair<size_t, msgpack_object>(2 + (size_t)bin8_size, msgpack_bin(bin8_size, start + 2));
                    }
                    case 0xc5: // bin 16
                    {    
                        // +--------+--------+--------+========+
                        // |  0xc5  |YYYYYYYY|YYYYYYYY|  data  |
                        // +--------+--------+--------+========+

                        uint16_t bin16_size;
                        std::memcpy(&bin16_size, start + 1, sizeof(bin16_size));
                        bin16_size = __bswap_16(bin16_size);

                        return std::make_pair<size_t, msgpack_object>((size_t)bin16_size, msgpack_bin(bin16_size, start + 3));

                    }
                    case 0xc6: // bin 32
                    {
                        // +--------+--------+--------+--------+--------+========+
                        // |  0xc6  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|  data  |
                        // +--------+--------+--------+--------+--------+========+
                        uint32_t bin32_size;
                        std::memcpy(&bin32_size, start + 1, sizeof(bin32_size));
                        bin32_size = __bswap_32(bin32_size);


                        return std::make_pair<size_t, msgpack_object>((size_t)bin32_size, msgpack_bin(bin32_size, start + 5));
                    }
                    case 0xc7: // ext 8
                    {


                    }
                    case 0xc8: // ext 16
                    {

                    }
                    case 0xc9: // ext 32
                    {

                    }
                    case 0xca:  // float
                    {
                        // +--------+--------+--------+--------+--------+
                        // |  0xca  |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
                        // +--------+--------+--------+--------+--------+


                    }
                    case 0xcb:  // double
                    {
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+
                        // |  0xcb  |YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+



                    }
                    case 0xcc:  // unsigned int  8
                    {
                        // +--------+--------+
                        // |  0xcc  |ZZZZZZZZ|
                        // +--------+--------+

                        return std::make_pair<size_t, msgpack_object>(2, (uint64_t)(*(start + 1)));

                    }
                    case 0xcd:  // unsigned int 16
                    {
                        // +--------+--------+--------+
                        // |  0xcd  |ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+

                        uint16_t value;
                        std::memcpy(&value, start + 1, sizeof(value));
                        value = __bswap_16(value);

                        return std::make_pair<size_t, msgpack_object>(3, (uint64_t)(value));

                    }
                    case 0xce:  // unsigned int 32
                    {
                        // +--------+--------+--------+--------+--------+
                        // |  0xce  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+--------+--------+

                        uint32_t value;
                        memcpy(&value, start + 1, sizeof(value));
                        value = __bswap_32(value);

                        return std::make_pair<size_t, msgpack_object>(5, (uint64_t)(value));

                    }
                    case 0xcf:  // unsigned int 64
                    {
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+
                        // |  0xcf  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+ 

                        uint64_t value;
                        memcpy(&value, start + 1, sizeof(value));
                        value = __bswap_64(value);

                        return std::make_pair<size_t, msgpack_object>(9, value);

                    }
                    case 0xd0:  // signed int  8
                    {
                        // +--------+--------+
                        // |  0xd0  |ZZZZZZZZ|
                        // +--------+--------+

                        return std::make_pair<size_t, msgpack_object>(2, (int64_t)(*(start + 1)));
                    }
                    case 0xd1:  // signed int 16
                    {
                        // +--------+--------+--------+
                        // |  0xd1  |ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+

                        int16_t value;
                        memcpy(&value, start + 1, sizeof(value));
                        value = __bswap_16(value);

                        return std::make_pair<size_t, msgpack_object>(3, (int64_t)value);


                    }
                    case 0xd2:  // signed int 32
                    {
                        // +--------+--------+--------+--------+--------+
                        // |  0xd2  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+--------+--------+
                        int32_t value;
                        memcpy(&value, start + 1, sizeof(value));
                        value = __bswap_32(value);

                        return std::make_pair<size_t, msgpack_object>(5, (int64_t)value);

                    }
                    case 0xd3:  // signed int 64
                    {
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+
                        // |  0xd3  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+
                        int64_t value;
                        memcpy(&value, start + 1, sizeof(value));
                        value = __bswap_64(value);

                        return std::make_pair<size_t, msgpack_object>(9, value);

                    }
                    case 0xd4:  // fixext 1
                    {

                    }
                    case 0xd5:  // fixext 2
                    {

                    }
                    case 0xd6:  // fixext 4
                    {

                    }
                    case 0xd7:  // fixext 8
                    {

                    }
                    case 0xd8:  // fixext 16
                    {

                    }

                    case 0xd9:  // str 8
                    {
                        // +--------+--------+========+
                        // |  0xd9  |YYYYYYYY|  data  |
                        // +--------+--------+========+

                        uint8_t str8_size;
                        std::memcpy(&str8_size, start + 1, sizeof(str8_size));

                        return std::make_pair<size_t, msgpack_object>(2 + (size_t)str8_size, msgpack_str(str8_size, (char *)(start + 2)));

                    }
                    case 0xda:  // str 16
                    {
                        // +--------+--------+--------+========+
                        // |  0xda  |ZZZZZZZZ|ZZZZZZZZ|  data  |
                        // +--------+--------+--------+========+

                        uint16_t str16_size;
                        std::memcpy(&str16_size, start + 1, sizeof(str16_size));
                        str16_size = __bswap_16(str16_size);

                        return std::make_pair<size_t, msgpack_object>(3 + (size_t)str16_size, msgpack_str(str16_size, (char *)(start + 3)));

                    }
                    case 0xdb:  // str 32
                    {
                        // +--------+--------+--------+--------+--------+========+
                        // |  0xdb  |AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|  data  |
                        // +--------+--------+--------+--------+--------+========+

                        uint32_t str32_size;
                        std::memcpy(&str32_size, start + 1, sizeof(str32_size));
                        str32_size = __bswap_32(str32_size);

                        return std::make_pair<size_t, msgpack_object>(5 + (size_t)str32_size, msgpack_str(str32_size, (char *)(start + 5)));

                    }
                    case 0xdc:  // array 16
                    {
                        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+
                        // |  0xdc  |YYYYYYYY|YYYYYYYY|    N objects    |
                        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+


                    }
                    case 0xdd:  // array 32
                    {
                        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
                        // |  0xdd  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|    N objects    |
                        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+

                    }
                    case 0xde:  // map 16
                    {
                        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+
                        // |  0xde  |YYYYYYYY|YYYYYYYY|    N*2 objects    |
                        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+

                        uint16_t nmb_elements;
                        std::memcpy(&nmb_elements, start + 1, sizeof(nmb_elements));
                        nmb_elements = __bswap_16(nmb_elements);

                        size_t bytes = skip_map(start + 3, nmb_elements);

                        return std::make_pair<size_t, msgpack_object>(3, msgpack_map(nmb_elements, bytes, start + 3));

                    }
                    case 0xdf:  // map 32
                    {
                        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
                        // |  0xdf  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|    N*2 objects  |
                        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+

                        uint16_t nmb_elements;
                        std::memcpy(&nmb_elements, start + 1, sizeof(nmb_elements));
                        nmb_elements = __bswap_32(nmb_elements);

                        size_t bytes = skip_map(start + 5, nmb_elements);
                        
                        return std::make_pair<size_t, msgpack_object>(5, msgpack_map(nmb_elements, bytes, start + 5));

                    }
                    default:
                    {
                         std::cerr << "Parsing error. Invalid type byte: " << *start << std::endl;
                    }
              }

         }
         case 0xa0 ... 0xbf: // fixstr
         {
            uint8_t fixstr_size = *start & 0b00011111;
            return std::make_pair<size_t, msgpack_object>(1 + (size_t)fixstr_size, msgpack_str(fixstr_size, (char *)(start + 1)));
         }
         case 0x90 ... 0x9f: // fix array
         {

         }
         case 0x80 ... 0x8f: // fix map
         {

            // +--------+~~~~~~~~~~~~~~~~~+
            // |1000XXXX|   N*2 objects   |
            // +--------+~~~~~~~~~~~~~~~~~+

            uint8_t nmb_elements = *start & 0b00001111;

            size_t bytes = skip_map(start + 1, nmb_elements)
            return std::make_pair<size_t, msgpack_object>(1, msgpack_map(nmb_elements, start + 1));
         }
         default:
         {
             std::cerr << "Parsing error. Invalid type byte: " << *start << std::endl;
         }
     }

    return std::make_pair<size_t, msgpack_object>(0, std::monostate());

}

uint8_t* Msgpack::find_map_key(const msgpack_map &map, const std::string &key)
{   
    // start = pointr to start of map
    // nmb_elements = number of elements in map
    // key = key to search for
    // we only care about the top level of the map, so we have to skip non-trivial objects that
    // do not have a matching key

    size_t map_offset = 0; //offset into the map
    size_t element_counter = 0; // current element being analysed
    uint8_t current_key;
    
    size_t nmb_elements = map.size;
    const uint8_t* start = map.data;

    // terminating condition: we are done when all of the elements have been exhausted.
    while (element_counter < nmb_elements)
    {
        current_key = *(start + map_offset);


        // currently only handling string keys
        if (current_key == TYPE_MASK::STR8 || (current_key >= 0b10100000 && current_key <= 0b10111111) ||
            current_key  == TYPE_MASK::STR16 || current_key == TYPE_MASK::STR32)
        {
            

        } else
        {
            map_offset += skip_object(start + map_offset);
        }
        


    }


    return nullptr;
}

size_t Msgpack::skip_object(const uint8_t* start)
{
     size_t bytes_read = 0;
     size_t offset = 0;

     switch (*start)
     {
         case 0x00 ... 0x7f: // positive fixnum
         {
            //  positive fixnum stores 7-bit positive integer
            // +--------+
            // |0XXXXXXX|
            // +--------+
            return 1;
         }
         case 0xe0 ... 0xff: // negative fixnum
         {
            // negative fixnum stores 5-bit negative integer
            // +--------+
            // |111YYYYY|
            // +--------+
            // TODO convert 5-bit negative integer to negative int64_t

            return 1;

         }
         case 0xc0 ... 0xdf: // variable length types
         {
              switch(*start) 
              {
                    case 0xc0:  // ???
                    case 0xc2:  // false
                    case 0xc3:  // true
                    {
                        return 1;
                    }
                    case 0xc4: // bin 8
                    {
                        // +--------+--------+========+
                        // |  0xc4  |XXXXXXXX|  data  |
                        // +--------+--------+========+

                        uint8_t bin8_size;
                        std::memcpy(&bin8_size, start + 1, sizeof(bin8_size));
                        return 2 + bin8_size;
                    }
                    case 0xc5: // bin 16
                    {    
                        // +--------+--------+--------+========+
                        // |  0xc5  |YYYYYYYY|YYYYYYYY|  data  |
                        // +--------+--------+--------+========+

                        uint16_t bin16_size;
                        std::memcpy(&bin16_size, start + 1, sizeof(bin16_size));
                        bin16_size = __bswap_16(bin16_size);

                        return 3 + bin16_size;
                    }
                    case 0xc6: // bin 32
                    {
                        // +--------+--------+--------+--------+--------+========+
                        // |  0xc6  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|  data  |
                        // +--------+--------+--------+--------+--------+========+
                        uint32_t bin32_size;
                        std::memcpy(&bin32_size, start + 1, sizeof(bin32_size));
                        bin32_size = __bswap_32(bin32_size);

                        return 5 + bin32_size;
                    }
                    case 0xc7: // ext 8
                    {


                    }
                    case 0xc8: // ext 16
                    {

                    }
                    case 0xc9: // ext 32
                    {

                    }
                    case 0xca:  // float
                    {
                        // +--------+--------+--------+--------+--------+
                        // |  0xca  |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX|
                        // +--------+--------+--------+--------+--------+


                    }
                    case 0xcb:  // double
                    {
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+
                        // |  0xcb  |YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+



                    }
                    case 0xcc:  // unsigned int  8
                    {
                        // +--------+--------+
                        // |  0xcc  |ZZZZZZZZ|
                        // +--------+--------+

                        return 2;
                    }
                    case 0xcd:  // unsigned int 16
                    {
                        // +--------+--------+--------+
                        // |  0xcd  |ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+

                        return  3;
                    }
                    case 0xce:  // unsigned int 32
                    {
                        // +--------+--------+--------+--------+--------+
                        // |  0xce  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+--------+--------+

                        return 5;
                    }
                    case 0xcf:  // unsigned int 64
                    {
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+
                        // |  0xcf  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+ 
                        return 9;
                    }
                    case 0xd0:  // signed int  8
                    {
                        // +--------+--------+
                        // |  0xd0  |ZZZZZZZZ|
                        // +--------+--------+
                        return 2;

                    }
                    case 0xd1:  // signed int 16
                    {
                        // +--------+--------+--------+
                        // |  0xd1  |ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+

                        return 3;


                    }
                    case 0xd2:  // signed int 32
                    {
                        // +--------+--------+--------+--------+--------+
                        // |  0xd2  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+--------+--------+

                        return 5;
                    }
                    case 0xd3:  // signed int 64
                    {
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+
                        // |  0xd3  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
                        // +--------+--------+--------+--------+--------+--------+--------+--------+--------+

                        return 9;

                    }
                    case 0xd4:  // fixext 1
                    {

                    }
                    case 0xd5:  // fixext 2
                    {

                    }
                    case 0xd6:  // fixext 4
                    {

                    }
                    case 0xd7:  // fixext 8
                    {

                    }
                    case 0xd8:  // fixext 16
                    {

                    }

                    case 0xd9:  // str 8
                    {
                        // +--------+--------+========+
                        // |  0xd9  |YYYYYYYY|  data  |
                        // +--------+--------+========+

                        uint8_t str8_size;
                        std::memcpy(&str8_size, start + 1, sizeof(str8_size));

                        return 2 + str8_size;
                    }
                    case 0xda:  // str 16
                    {
                        // +--------+--------+--------+========+
                        // |  0xda  |ZZZZZZZZ|ZZZZZZZZ|  data  |
                        // +--------+--------+--------+========+

                        uint16_t str16_size;
                        std::memcpy(&str16_size, start + 1, sizeof(str16_size));
                        str16_size = __bswap_16(str16_size);

                        return 3 + str16_size;
                    }
                    case 0xdb:  // str 32
                    {
                        // +--------+--------+--------+--------+--------+========+
                        // |  0xdb  |AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|  data  |
                        // +--------+--------+--------+--------+--------+========+

                        uint32_t str32_size;
                        std::memcpy(&str32_size, start + 1, sizeof(str32_size));
                        str32_size = __bswap_32(str32_size);

                        return 5 + str32_size;
                    }
                    case 0xdc:  // array 16
                    {
                        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+
                        // |  0xdc  |YYYYYYYY|YYYYYYYY|    N objects    |
                        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+


                    }
                    case 0xdd:  // array 32
                    {
                        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
                        // |  0xdd  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|    N objects    |
                        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+

                    }
                    case 0xde:  // map 16
                    {
                        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+
                        // |  0xde  |YYYYYYYY|YYYYYYYY|    N*2 objects    |
                        // +--------+--------+--------+~~~~~~~~~~~~~~~~~+

                        auto [read, map16] = parse_data(start);

                        ASSERT(read == 3);

                        return read + skip_map(std::get<msgpack_map>(map16));
                    }
                    case 0xdf:  // map 32
                    {
                        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
                        // |  0xdf  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|    N*2 objects  |
                        // +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+

                        auto [read, map32] = parse_data(start);

                        ASSERT(read == 5);

                        return read + skip_map(std::get<msgpack_map>(map32));
                    }
                    default:
                    {
                         std::cerr << "Parsing error. Invalid type byte: " << *start << std::endl;
                    }
              }

         }
         case 0xa0 ... 0xbf: // fixstr
         {
            uint8_t fixstr_size = *start & 0b00011111;
            return std::make_pair<size_t, msgpack_object>(1 + (size_t)fixstr_size, msgpack_str(fixstr_size, (char *)(start + 1)));
         }
         case 0x90 ... 0x9f: // fix array
         {

         }
         case 0x80 ... 0x8f: // fix map
         {

         }
         default:
         {
             std::cerr << "Parsing error. Invalid type byte: " << *start << std::endl;
         }
     }

    return 0;
}

size_t Msgpack::skip_map(const msgpack_map &map)
{
    size_t offset = 0; // offset into the map data
    size_t element_count = 0;

    while (element_count < map.size * 2)
    {
        offset += skip_object(map.data + offset);
        
        element_count++;
    }

    return offset;
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
        
        // uint16_t nmb_elements;
        // std::memcpy(&nmb_elements, this->data + 1, 2);

        // how to parse a map
        // read a byte, resolve the type, 
        // if type is string, check for equality with 'key'
        // if not equal, skip forward to the next key
        // uint8_t* value = find_map_key(this->data + 3, nmb_elements, key);

    
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