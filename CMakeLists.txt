cmake_minimum_required(VERSION 3.16)
project(Circuit_Simulator VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native -flto")
enable_testing()

# Set bin as output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add Eigen Library
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE lib/eigen)

# Add Include Library
include_directories(include)

# Main Executable - Simulator
add_executable(simulator src/main.cpp)
target_link_libraries(simulator eigen)

# Test Executables
add_executable(test_math test/test_math.cpp)
add_executable(test_parser test/test_parser.cpp)
add_executable(test_transient test/test_transient.cpp)
add_executable(test_eigen test/test_eigen.cpp)

target_link_libraries(test_math eigen)
target_link_libraries(test_parser eigen)
target_link_libraries(test_transient eigen)
target_link_libraries(test_eigen eigen)

add_test(NAME TEST_MATH COMMAND test_math WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_test(NAME TEST_PARSER COMMAND test_parser WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_test(NAME TEST_TRANSIENT COMMAND test_transient WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})