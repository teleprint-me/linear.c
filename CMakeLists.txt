# Root CMakeLists.txt

cmake_minimum_required(VERSION 3.14)
project(linear VERSION 0.1.0 DESCRIPTION "ANSI C library for Linear Algebra")

# Set C standard rules
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Enable Shared Objects
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# Enable Multi-threaded support
option(LINEAR_THREAD "Enable Thread support" OFF)
option(LINEAR_VULKAN "Enable Vulkan support" OFF)

# Setup submodule, module, and test names
set(SUBMODULES logger lehmer)
set(MODULES vector matrix)

# Set the output directory for built binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add dependencies
foreach(MOD IN LISTS SUBMODULES)
    add_subdirectory(mods/${MOD})
endforeach()

# Collect all source files into lists
foreach(MOD IN LISTS MODULES)
    list(APPEND LIB_SOURCES "src/${MOD}.c")
    list(APPEND LIB_HEADERS "include/${MOD}.h")
endforeach()

if (LINEAR_VULKAN)
    find_package(Vulkan COMPONENTS glslc REQUIRED)
endif()

# Add the library using the collected sources
add_library(linear ${LIB_SOURCES})

set_target_properties(
    linear PROPERTIES
    VERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${LIB_HEADERS}"
)

target_include_directories(linear PUBLIC include)
target_link_libraries(linear PUBLIC logger float_is_close lehmer)

# Add test executables
foreach(MOD IN LISTS MODULES)
    add_executable("test_linear_${MOD}" "tests/test_linear_${MOD}.c")
endforeach()

# Link test executables
foreach(MOD IN LISTS MODULES)
    target_link_libraries("test_linear_${MOD}" m linear)
endforeach()

# Set the output directory for the test executables
set_target_properties(
    test_linear_vector test_linear_matrix # [<targets>]...
    PROPERTIES # PROPERTIES [<prop1> <value1>]...
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

# Install rules for the library
install(
    TARGETS linear
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)

# NOTE: Slow tests should always be last
enable_testing()
foreach(MOD IN LISTS MODULES)
    add_test(
        NAME "test_linear_${MOD}"
        COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_linear_${MOD}"
    )    
endforeach()

link_directories(${CMAKE_BINARY_DIR})
add_subdirectory(examples)
