cmake_minimum_required(VERSION 3.8)

enable_testing()

find_package(Gtest REQUIRED)

add_executable(msgpacksearch_unittest
        test_msgpacksearch.cpp)

target_link_libraries(msgpacksearch_unittest PUBLIC
        msgpacksearch
        Gtest::Gtest
        Gtest::Main
        )

add_test(
        NAME msgpacksearch_unittest 
        COMMAND msgpacksearch_unittest
)
