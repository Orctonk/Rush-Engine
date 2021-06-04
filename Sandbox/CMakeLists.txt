cmake_minimum_required(VERSION 3.17)

project(Sandbox VERSION 1.0)

set(SANDBOX_SOURCE_DIR "src")

set(SANDBOX_SOURCES 
    ${SANDBOX_SOURCE_DIR}/main.cpp
    ${SANDBOX_SOURCE_DIR}/SandboxLayer.cpp
)

add_executable(Sandbox ${SANDBOX_SOURCES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(WINDOWS)
    add_compile_definitions(RUSH_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(RUSH_LINUX)
else()
    add_compile_definitions(RUSH_APPLE)  
endif()

target_link_libraries(Sandbox PUBLIC Rush)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:Rush>
    $<TARGET_FILE_DIR:Sandbox>)