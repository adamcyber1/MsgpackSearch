cmake_minimum_required(VERSION 3.8)
project(msgpacksearch CXX)

set(SOURCE_FILES
    msgpacksearch.h
    msgpacksearch.cpp
    error.h
    types.h)

add_library(msgpacksearch ${SOURCE_FILES})

install(TARGETS msgpacksearch DESTINATION ${MSGPACKSEARCH_INSTALL_LIB_DIR})
install(FILES msgpacksearch.h DESTINATION ${MSGPACKSEARCH_INSTALL_INCLUDE_DIR})
