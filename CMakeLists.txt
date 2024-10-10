# Main project MakeLists.txt file.
cmake_minimum_required(VERSION 3.15)
project(feather C)


include_directories(include include/insc)
find_package(SDL REQUIRED)

file(GLOB_RECURSE SRCS src/*.c)
add_library(feather ${SRCS})

target_include_directories(feather PUBLIC include/*)

target_link_libraries(feather PRIVATE SDL::SDL)

set_target_properties(
    feather PROPERTIES PUBLIC_HEADER 
    "include/feather.h"
)

install(TARGETS feather)
