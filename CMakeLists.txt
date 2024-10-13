# Main project MakeLists.txt file.
cmake_minimum_required(VERSION 3.15)
project(feather C)

# Required external libraries.
find_package(SDL REQUIRED)
find_package(tllist REQUIRED)

include_directories(feather PUBLIC include)

file(GLOB_RECURSE SRCS src/*.c)
add_library(feather ${SRCS})

target_link_libraries(feather PRIVATE SDL::SDL tllist::tllist)

file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
set_target_properties(feather PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

install(TARGETS feather)
