cmake_minimum_required(VERSION 3.16)
project(Circuit_Simulator VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native -flto")
enable_testing()

# Set bin as output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add Symbolic Library
add_library(symbolic INTERFACE)
target_include_directories(symbolic INTERFACE lib/symbolic)

# Add Include Library
include_directories(include)

# Main Executable - Simulator
# add_executable(simulator src/main.cpp)
# target_link_libraries(simulator symbolic)

# Test Executables
add_executable(test_math test/test_math.cpp)
add_executable(test_parser test/test_parser.cpp)
target_link_libraries(test_math symbolic)
target_link_libraries(test_parser symbolic)

add_test(NAME TEST_MATH COMMAND test_math WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_test(NAME TEST_PARSER COMMAND test_parser WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
