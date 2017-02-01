cmake_minimum_required(VERSION 3.6)
project(yaplc)

set(CMAKE_CXX_STANDARD 11)

include_directories("src")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.c")

list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/regex/slre/unit_test.c)

add_executable(yaplc ${SOURCE_FILES})