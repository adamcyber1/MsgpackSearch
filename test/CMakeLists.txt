cmake_minimum_required(VERSION 3.8)

enable_testing()

add_executable(msgpacksearch_unittest
        test_msgpacksearch.cpp)

target_link_libraries(msgpacksearch_unittest PUBLIC
        gtest_main
        msgpacksearch
        )

add_test(
        NAME msgpacksearch_unittest 
        COMMAND msgpacksearch_unittest
)