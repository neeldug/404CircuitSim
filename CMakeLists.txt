cmake_minimum_required(VERSION 3.16)
project(Circuit_Simulator VERSION 1.0.0)
enable_testing()

# Set bin as output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add Symbolic Library
add_library(symbolic INTERFACE)
target_include_directories(symbolic INTERFACE lib/symbolic)

# Add Include Library
include_directories(include)

# Main Executable - Simulator
add_executable(simulator src/main.cpp)
target_link_libraries(simulator symbolic)

# Test Executables
add_executable(test_math test/test_math.cpp)
target_link_libraries(test_math symbolic)
add_test(NAME TEST_MATH COMMAND test_math WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})