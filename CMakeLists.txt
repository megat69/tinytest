# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.12)
project(tinytest)

# Variables
set(COMPILE_DOXYGEN OFF)

# Includes
include_directories(src/)

# If this is not being run by another CMake (e.g. FetchContent)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # Base project settings
    set(CMAKE_BUILD_TYPE Debug)

    # Sources
    set(tinytest
        src/tinytest.hpp
    )

    # Applications
    # -- TEST --
    add_executable(test_tinytest ${tinytest} test/tinytest/letters.hpp test/tinytest/test_letters.cpp)
endif()

# Doxygen
if (${COMPILE_DOXYGEN})
    add_subdirectory("doc")
endif()
