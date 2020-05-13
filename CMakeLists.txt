cmake_minimum_required(VERSION 3.16)

#set bin director
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# set the project name
project(Circuit_Simulator)

# add the executable
add_library(
    circuit
    src/components/circuitStructure.cpp
    src/components/linearDevices.cpp
)


target_include_directories(circuit PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/.")
target_include_directories(circuit PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/maths")
add_executable(simulator src/main.cpp)
target_link_libraries(simulator PRIVATE circuit)