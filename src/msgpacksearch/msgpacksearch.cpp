#include "msgpacksearch.h"

namespace msgpacksearch
{

Msgpack::Msgpack(const uint8_t *data, size_t length) : data(data), size(length), offset(0) {}

Msgpack::Msgpack(const char *data, size_t length) : Msgpack((uint8_t *)data, length) {}

Msgpack::Msgpack(const std::vector<uint8_t> &data) : Msgpack(data.data(), data.size()) {}

Msgpack::Msgpack(const std::vector<char> &data) : Msgpack((uint8_t *)data.data(), data.size()) {}

obj Msgpack::get(const std::string &key) 
{
    if (*this->data == TYPE::MAP16 || *this->data == TYPE::MAP32)
    {
        //find_key(start, end, )
    } else
    {
        // return exception
        return obj();
    }
    
    return obj();
}


obj Msgpack::get(const int index)
{
    return obj();
}


} // namespace msgpacksearch