cmake_minimum_required(VERSION 3.10)
project(msgpacksearch-cli)

add_subdirectory(msgpacksearch)
set(SOURCE_FILES main.cpp)

add_executable(msgpacksearch-cli ${SOURCE_FILES})
target_link_libraries(msgpacksearch-cli msgpacksearch)
install(TARGETS msgpacksearch-cli DESTINATION ${MSGPACKSEARCH_INSTALL_BIN_DIR})
