cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party/googletest)
enable_testing()

add_executable(lab3
    src/main.cpp
    src/newfile.cpp
    cli/interface.cpp          
)

add_executable(tests
    tests/test_dynamic_array.cpp
    tests/test_linked_list.cpp
    tests/test_sequence.cpp
    tests/test_queue.cpp
    tests/test_deque.cpp
    tests/test_stack.cpp
    tests/test_vector.cpp
)

target_include_directories(lab3 PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/cli
)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tests gtest gtest_main)
add_test(NAME Lab3Tests COMMAND tests)