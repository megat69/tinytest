# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.12)
project(scrabble)
set(CMAKE_BUILD_TYPE Debug)

# Sources
set(tinytest
    src/tinytest.hpp
)
include_directories(src/)

# Applications
# -- TEST --
add_executable(test_letters ${tiytest} test/letters.hpp test/test_letters.cpp)
