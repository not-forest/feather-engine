cmake_minimum_required(VERSION 3.15)
project(PackageTest C)
find_package(feather CONFIG REQUIRED)

add_executable(minimal src/minimal.c)

target_link_libraries(minimal feather::feather)
