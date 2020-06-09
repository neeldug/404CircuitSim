cmake_minimum_required(VERSION 3.16)
project(Circuit_Simulator VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -march=native -flto")
enable_testing()

# Set bin as output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add Eigen Library
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE lib/Eigen)

# Add hayai benchmarking 
add_library(hayai INTERFACE)
target_include_directories(hayai INTERFACE lib/hayai)

# Add Include Library
include_directories(include)

# Main Executable - Simulator
add_executable(simulator src/main.cpp)
target_link_libraries(simulator eigen)

# Installations
install(TARGETS simulator RUNTIME DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/bin/plot.py DESTINATION bin)

# Test Executables
add_subdirectory(test)

# Cpack
include(CPack)

