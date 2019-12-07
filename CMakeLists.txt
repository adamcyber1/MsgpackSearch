cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

if (NOT (UNIX))
  message(FATAL_ERROR "unsupported target platform...windows isn't supported yet")
endif()

project(msgpacksearch)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# languages
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -O0 -Wall")

# options
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_TESTS "Build Tests" OFF)

if(BUILD_SHARED_LIBS)
    message("BUILD_SHARED_LIBS: ON")
else()
    message("BUILD_SHARED_LIBS: OFF")
endif()

if(BUILD_TESTS)
    enable_testing()
    message("BUILD_TESTS: ON")
else()
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