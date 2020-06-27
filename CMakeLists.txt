cmake_minimum_required (VERSION 2.6)
project (leptjson_test)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -std=c++11 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -std=c++11")

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall")
endif()

add_library(ctx_json ctx_json_3.cpp)
add_executable(ctx_json_test ctx_json_test_3.cpp)
target_link_libraries(ctx_json_test ctx_json)

