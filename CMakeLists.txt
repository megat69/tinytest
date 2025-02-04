# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.12)
project(scrabble)
set(CMAKE_BUILD_TYPE Debug)

# Sources
set(tinytest
    src/tinytest1.hpp
    src/tinytest2.hpp
)
include_directories(src/)

# Applications
# -- TEST --
add_executable(test_tinytest1 ${tinytest} test/tinytest1/letters.hpp test/tinytest1/test_letters.cpp)
add_executable(test_tinytest2 ${tinytest} test/tinytest2/letters.hpp test/tinytest2/test_letters.cpp)
