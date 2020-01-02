# FindGtest.cmake
#
# Finds the gtest library
#
# This will define the following variables
#
#  Gtest_FOUND             System has gtest libs/headers
#  Gtest_LIBRARIES         The gtest library/libraries
#  GtestMain_LIBRARIES     The gtest_main library/libraries
#  Gtest_INCLUDE_DIR       The location of gtest headers
#
# and the following imported targets
#
#   Gtest::Gtest
#

find_library(Gtest_LIBRARIES
        NAMES gtest
        )

find_library(GtestMain_LIBRARIES
        NAMES gtest_main
        )

find_path(Gtest_INCLUDE_DIR
        NAMES gtest/gtest.h
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Gtest DEFAULT_MSG
        Gtest_LIBRARIES
        Gtest_INCLUDE_DIR
        )
find_package_handle_standard_args(GtestMain DEFAULT_MSG
        GtestMain_LIBRARIES
        )

mark_as_advanced(
        GtestMain_LIBRARIES
        Gtest_LIBRARIES
        Gtest_INCLUDE_DIR
)

if(Gtest_FOUND AND NOT TARGET Gtest::Gtest)
    add_library(Gtest::Gtest INTERFACE IMPORTED)
    set_target_properties(Gtest::Gtest PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${Gtest_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${Gtest_LIBRARIES}"
            )
endif()

if(GtestMain_FOUND AND NOT TARGET Gtest::Main)
    add_library(Gtest::Main INTERFACE IMPORTED)
    set_target_properties(Gtest::Main PROPERTIES
            INTERFACE_LINK_LIBRARIES "${GtestMain_LIBRARIES}"
            )
endif()