add_executable(test_math test_math.cpp)
add_executable(test_parser test_parser.cpp)
add_executable(test_transient test_transient.cpp)
add_executable(test_eigen test_eigen.cpp)

target_link_libraries(test_math eigen)
target_link_libraries(test_parser eigen)
target_link_libraries(test_transient eigen)
target_link_libraries(test_eigen eigen)

add_test(NAME TEST_MATH COMMAND test_math WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_test(NAME TEST_PARSER COMMAND test_parser WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_test(NAME TEST_TRANSIENT COMMAND test_transient WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
