cmake_minimum_required(VERSION 3.16)
project(Circuit_Simulator VERSION 1.0.0)

# Set bin as output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add Symbolic Library
add_library(symbolic INTERFACE)
target_include_directories(symbolic INTERFACE lib/symbolic)

# Main Executable - Simulator
add_executable(simulator src/main.cpp)
target_include_directories(simulator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(simulator symbolic)