#include "msgpacksearch.h"

namespace msgpacksearch
{

Msgpack::Msgpack(const uint8_t *data, size_t length) : data(data), size(length), offset(0)
{
}

Msgpack::Msgpack(const char *data, size_t length) : Msgpack((uint8_t *)data, length)
{
}

Msgpack::Msgpack(const std::vector<uint8_t> &data) : Msgpack(data.data(), data.size())
{
}

Msgpack::Msgpack(const std::vector<char> &data) : Msgpack((uint8_t *)data.data(), data.size())
{
}



} // namespace msgpacksearch