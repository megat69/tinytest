# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.12)
project(scrabble)
set(CMAKE_BUILD_TYPE Debug)

# Variables
set(COMPILE_DOXYGEN OFF)

# Sources
set(tinytest
    src/tinytest.hpp
)
include_directories(src/)

# Applications
# -- TEST --
add_executable(test_tinytest ${tinytest} test/tinytest/letters.hpp test/tinytest/test_letters.cpp)

# Doxygen
if (${COMPILE_DOXYGEN})
    add_subdirectory("doc")
endif()
