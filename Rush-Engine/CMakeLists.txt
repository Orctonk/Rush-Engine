cmake_minimum_required(VERSION 3.10)

option(OPENGL "Use OpenGL for rendering" ON)
option(RUSH_BUILD_SHARED "Build rush as a shared library" ON)

project(Rush VERSION 1.0)

set(RUSH_INCLUDE_DIR "include")
set(RUSH_SOURCE_DIR "src")
set(RUSH_CMAKE_CONFIG_DIR "cmake-config")
set(RUSH_OUT_DIR "out")

set(RUSH_SOURCES ${RUSH_SOURCE_DIR}/print.cpp)

if(RUSH_BUILD_SHARED)
    add_library(Rush SHARED ${RUSH_SOURCES})
else()
    add_library(Rush STATIC ${RUSH_SOURCES})
endif()

configure_file(${RUSH_CMAKE_CONFIG_DIR}/RushVersion.h.in ${RUSH_INCLUDE_DIR}/RushVersion.h)
target_include_directories(Rush PUBLIC ${RUSH_INCLUDE_DIR})
target_include_directories(Rush PUBLIC ${PROJECT_BINARY_DIR}/${RUSH_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if(WINDOWS)
    add_compile_definitions(Rush RUSH_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(Rush RUSH_LINUX)
else()
    add_compile_definitions(Rush RUSH_APPLE)  
endif()