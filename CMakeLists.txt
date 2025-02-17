# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.12)
project(scrabble)
set(CMAKE_BUILD_TYPE Debug)

# Variables
set(COMPILE_DOXYGEN OFF)

# Packages
if(${COMPILE_DOXYGEN})
find_package(Doxygen REQUIRED)
endif()

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
    add_custom_target(
            doc ALL
            COMMAND
            ${DOXYGEN_EXECUTABLE} doc/doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
    )
endif()
