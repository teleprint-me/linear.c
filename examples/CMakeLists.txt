# Examples CMakeLists.txt

# Add linear related examples
set(LINEAR_SOURCES
    vector_simple
)

# Create executables for linear examples
foreach(example IN LISTS LINEAR_SOURCES)
    add_executable(${example} ${PROJECT_SOURCE_DIR}/examples/${example}.c)
    target_link_libraries(${example} m linear)
    target_include_directories(${example} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    set_target_properties(${example} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/examples)
endforeach()
