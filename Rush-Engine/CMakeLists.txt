cmake_minimum_required(VERSION 3.17)

option(RUSH_OPENGL "Use OpenGL for rendering" ON)
option(RUSH_BUILD_SHARED "Build rush as a shared library" ON)

project(Rush VERSION 1.0)

set(RUSH_INCLUDE_DIR "include")
set(RUSH_SOURCE_DIR "src")
set(RUSH_DEPS_DIR "Libraries")
set(RUSH_CMAKE_CONFIG_DIR "cmake-config")
set(RUSH_OUT_DIR "out")

set(RUSH_SOURCES    ${RUSH_SOURCE_DIR}/core/Application.cpp
                    ${RUSH_SOURCE_DIR}/core/EntryPoint.cpp
                    ${RUSH_SOURCE_DIR}/core/Logger.cpp
                    ${RUSH_SOURCE_DIR}/window/AbstractWindow.cpp)

if(RUSH_OPENGL)
    list(APPEND RUSH_SOURCES 
                    ${RUSH_SOURCE_DIR}/window/GLFWWindow.cpp)
else()
    message(FATAL_ERROR "No window manager provided!")
endif()

if(RUSH_BUILD_SHARED)
    add_library(Rush SHARED ${RUSH_SOURCES})
else()
    add_library(Rush STATIC ${RUSH_SOURCES})
endif()

configure_file(${RUSH_CMAKE_CONFIG_DIR}/RushVersion.h.in RushVersion.h)
target_include_directories(Rush PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(Rush INTERFACE ${RUSH_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if(WINDOWS)
    message(WARNING "Windows has not yet been tested!")
    add_compile_definitions(RUSH_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(RUSH_LINUX)
    target_link_libraries(Rush pthread)
else()
    message(FATAL_ERROR "OSX is not yet supported")
    add_compile_definitions(RUSH_APPLE)  
endif()

if(RUSH_OPENGL)
    add_compile_definitions(RUSH_OPENGL)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(${RUSH_DEPS_DIR}/glfw-3.2.1)
    target_link_libraries(Rush glfw)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Rush OpenGL::GL)

    add_subdirectory(${RUSH_DEPS_DIR}/glad)
    target_link_libraries(Rush glad)
endif()