cmake_minimum_required(VERSION 3.12)

project(OpenGL)

option(DEFAULT_DEBUG "Set Debug as default build type" ON)

if(NOT CMAKE_BUILD_TYPE AND DEFAULT_DEBUG)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/external)
include_directories(${PROJECT_SOURCE_DIR}/external/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

find_library(glfw glfw3 ${PROJECT_SOURCE_DIR}/lib)

aux_source_directory(${PROJECT_SOURCE_DIR}/external/glad/src SRC_GLAD)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

add_executable(renderer main.cpp ${SRC_LIST} ${SRC_GLAD})

target_precompile_headers(renderer PRIVATE ${PROJECT_SOURCE_DIR}/include/preheader.h)
target_link_libraries(renderer ${glfw})