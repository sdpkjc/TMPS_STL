cmake_minimum_required (VERSION 2.6)
project (TMPS_STL)
add_definitions("-Wall -pedantic -std=c++11")
# if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
#     set(CMAKE_C_FLAGS "${CMAKE_CPP_FLAGS} -std=c++17 -pedantic -Wall")
# endif()

add_executable(TMPS_STL_test
    test.cpp
    allocator.cpp
)
# add_library(a a.cpp)
# add_executable(a_test test.cpp)
# target_link_libraries(a_test a)
