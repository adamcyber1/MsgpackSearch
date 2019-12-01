cmake_minimum_required(VERSION 3.10)
project(msgpacksearch C CXX)

set(SOURCE_FILES
    msgpacksearch.h
    msgpacksearch.cpp
)

add_library(msgpacksearch SHARED STATIC ${SOURCE_FILES})

install(TARGETS msgpacksearch DESTINATION ${MSGPACKSEARCH_INSTALL_LIB_DIR})
install(FILES msgpacksearch.h DESTINATION ${MSGPACKSEARCH_INSTALL_INCLUDE_DIR})
