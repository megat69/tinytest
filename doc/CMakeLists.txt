find_package(Doxygen REQUIRED)

add_custom_target(
    doc ALL
    COMMAND
    ${DOXYGEN_EXECUTABLE} ./doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)