cmake_minimum_required(VERSION 3.14)
project(linear VERSION 0.1.0 DESCRIPTION "ANSI C library for Linear Algebra")

# Set C standard rules
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Add subdirectory for required dependencies
add_subdirectory(mods)

# Main library
add_library(
    linear
    src/vector.c
    src/matrix.c # work in progress
    # todo: add tensor support
)

set_target_properties(
    linear # [<targets>]...
    PROPERTIES # PROPERTIES [<prop1> <value1>]...
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER include/vector.h
    PUBLIC_HEADER include/matrix.h
)

target_include_directories(linear PUBLIC include)
target_link_libraries(linear PUBLIC logger)
target_link_libraries(linear PUBLIC float_is_close)

# Set the output directory for built binaries
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Add test executables
add_executable(test_linear_vector tests/test_linear_vector.c)

# Link test executables
target_link_libraries(test_linear_vector m linear)
target_link_libraries(test_linear_matrix m linear)

# Set the output directory for the test executables
set_target_properties(
    test_linear_vector test_linear_matrix # [<targets>]...
    PROPERTIES # PROPERTIES [<prop1> <value1>]...
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# Install rules for the library
install(
    TARGETS linear
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# Add custom targets to run tests
#
# NOTE: Custom targets do not seem to be able to be simplified
# due to the nature of their specificity.
add_custom_target(
    run_test_linear_vector
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/test_linear_vector
    DEPENDS test_linear_vector
    COMMENT "Running tests for lehmer_generate"
)
add_custom_target(
    run_test_linear_matrix
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/test_linear_matrix
    DEPENDS test_linear_matrix
    COMMENT "Running tests for lehmer_api"
)

# NOTE: Slow tests should always be last
enable_testing()
add_test(
    NAME test_linear_vector
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/test_linear_vector
)
add_test(
    NAME test_linear_matrix
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/test_linear_matrix
)
