cmake_minimum_required(VERSION 3.15)
project(PackageTest C)
find_package(feather CONFIG REQUIRED)

add_executable(minimal src/minimal.c)
add_executable(layer_counter src/layer_counter.c)
#add_executable(resource src/resource.c)

target_link_libraries(minimal feather::feather)
target_link_libraries(layer_counter feather::feather)
#target_link_libraries(resource feather::feather)
