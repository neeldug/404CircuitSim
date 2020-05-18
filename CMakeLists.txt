cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

project(Circuit_Simulator)

add_library(symbolic INTERFACE)
target_include_directories(symbolic INTERFACE lib/symbolic)

add_executable(simulator src/main.cpp src/circuit/circuit.cpp)
target_include_directories(simulator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(simulator symbolic)