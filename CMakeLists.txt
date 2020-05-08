cmake_minimum_required(VERSION 3.16)

#set bin director
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# set the project name
project(Circuit_Simulator)

# add the executable
add_executable(simulator src/main.cpp)