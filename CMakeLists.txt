cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

if (NOT (UNIX))
  message(FATAL_ERROR "unsupported target platform...windows isn't supported yet")
endif()

project(msgpacksearch)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7)
    message(FATAL_ERROR "Require at least gcc-7")
endif()

# languages
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -O0 -Wall -fPIC")

# options
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_TESTS "Build Tests" ON)

if(BUILD_SHARED_LIBS)
    message("BUILD_SHARED_LIBS: ON")
else()
    message("BUILD_SHARED_LIBS: OFF")
endif()

if(BUILD_TESTS)
    enable_testing()
    message("BUILD_TESTS: ON")
else()
    enable_testing()
    message("BUILD_TESTS: OFF")
endif()

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(MSGPACKSEARCH_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/build/include)
set(MSGPACKSEARCH_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/build/bin)
set(MSGPACKSEARCH_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(MSGPACKSEARCH_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/msgpacksearch)

include_directories(${MSGPACKSEARCH_INSTALL_INCLUDE_DIR})
include_directories(${MSGPACKSEARCH_HEADERS_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)

add_subdirectory(test)
