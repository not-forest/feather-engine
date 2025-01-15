cmake_minimum_required(VERSION 3.15)
project(PackageTest C)

# Import the feather static library
add_library(feather STATIC IMPORTED)
set_target_properties(feather PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/../include"
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../libfeather.a"
)

# Find required libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(cglm REQUIRED)

# Link dependencies to 'feather'
target_link_libraries(feather INTERFACE SDL2::SDL2 SDL2_mixer::SDL2_mixer SDL2_image::SDL2_image OpenGL::OpenGL SDL2_ttf::SDL2_ttf cglm::cglm m)

# Automatically find all test source files in src/
file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

# Iterate over each source file to create an executable and link it with the feather library
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Extract the file name without extension to name the executable
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} feather)
    
    # Add the executable as a test (requires CTest)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# Enable testing
enable_testing()

# Add Emscripten build configurations
if(EMSCRIPTEN)
    message(STATUS "Building with Emscripten")

    # Define Emscripten-specific options
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # Outputs .html files for web
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2 -s USE_WEBGL2=1 -s FETCH=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun")

    foreach(TEST_SOURCE ${TEST_SOURCES})
        # Extract the file name without extension to name the executable
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        
        # Create WebAssembly executables
        add_executable(${TEST_NAME}_web ${TEST_SOURCE})
        target_link_libraries(${TEST_NAME}_web feather)
    endforeach()
endif()

