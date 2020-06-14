cmake_minimum_required(VERSION 3.10)

project(Sandbox VERSION 1.0)

set(SANDBOX_SOURCE_DIR "src")

set(SANDBOX_SOURCES ${SANDBOX_SOURCE_DIR}/main.cpp)

add_executable(Sandbox ${SANDBOX_SOURCES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if(WINDOWS)
    add_compile_definitions(RUSH_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(RUSH_LINUX)
else()
    add_compile_definitions(RUSH_APPLE)  
endif()

target_link_libraries(Sandbox PUBLIC Rush)