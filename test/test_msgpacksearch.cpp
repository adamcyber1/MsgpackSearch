#include <utility>
#include <variant>

#include <gtest/gtest.h>

#include "msgpacksearch/msgpacksearch.h"

using namespace msgpacksearch;

TEST(parse, SimpleTypes)
{ 
    std::vector<uint8_t> data;
    Msgpack msgpck(data.data(), 0);

    data = {0x7f}; // 127
    auto [read, obj] =  msgpck.parse_data(data.data());

    EXPECT_EQ(1, read);
    EXPECT_EQ(127, std::get<uint64_t>(obj));

    data = {0xc2}; // false
    std::tie(read, obj) =  msgpck.parse_data(data.data());
    EXPECT_EQ(1, read);
    EXPECT_EQ(false, std::get<bool>(obj));

    data = {0xc3}; // true
    std::tie(read, obj) =  msgpck.parse_data(data.data());
    EXPECT_EQ(1, read);
    EXPECT_EQ(true, std::get<bool>(obj));

    data = {0xcc, 0x10}; // uint8 -> 16
    std::tie(read, obj) =  msgpck.parse_data(data.data());
    EXPECT_EQ(2, read);
    EXPECT_EQ(16, std::get<uint64_t>(obj));

    data = {0xcd, 0x12, 0x34}; // uint16 -> 0x12 0x34
    std::tie(read, obj) =  msgpck.parse_data(data.data());
    EXPECT_EQ(3, read);
    EXPECT_EQ(0x1234, std::get<uint64_t>(obj));

    data = {0xd3, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01}; // int64
    std::tie(read, obj) =  msgpck.parse_data(data.data());
    EXPECT_EQ(9, read);
    EXPECT_EQ(0x0000000100000001, std::get<int64_t>(obj));
}

TEST (parse, Binary)
{
    std::vector<uint8_t> data;
    Msgpack msgpck(data.data(), 0);

    data = {0xd9, 0x05, 'h', 'e', 'l', 'l', 'o'}; //str8 -> hello
    auto [read, obj] =  msgpck.parse_data(data.data());
    auto str = std::get<msgpack_str>(obj);
    std::string cpp_str(str.data, str.size);
    EXPECT_EQ(7, read);
    EXPECT_EQ("hello", cpp_str);

    /*
    data = {0xa5, 'h', 'e', 'l', 'l', 'o'}; // fixstr -> hello
    std::tie(read, obj) =  msgpck.parse_data(data.data());
    str = std::get<msgpack_str>(obj);
    cpp_str = std::string(str.data, str.size);
    EXPECT_EQ(6, read);
    EXPECT_EQ("hello", cpp_str);
    */

    data = {0xda, 0x00, 0x05, 'h', 'e', 'l', 'l', 'o'}; // str16 -> hello
    std::tie(read, obj) =  msgpck.parse_data(data.data());
    str = std::get<msgpack_str>(obj);
    cpp_str = std::string(str.data, str.size);
    EXPECT_EQ(8, read);
    EXPECT_EQ("hello", cpp_str);
}


TEST(config, second)
{
    EXPECT_EQ(1, 1);
}