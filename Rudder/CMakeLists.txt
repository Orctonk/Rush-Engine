cmake_minimum_required(VERSION 3.10)

project(Rudder VERSION 1.0)

set(RUDDER_SOURCE_DIR "src")
set(RUDDER_DEPS_DIR "Libraries")

set(RUDDER_SOURCES 
        ${RUDDER_SOURCE_DIR}/EditorApplication.cpp
        ${RUDDER_SOURCE_DIR}/EditorLayer.cpp
    )

add_executable(Rudder ${RUDDER_SOURCES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory("${RUDDER_DEPS_DIR}/imgui_entt_entity_editor")
target_link_libraries(Rudder PUBLIC imgui_entt_entity_editor)

if(WINDOWS)
    add_compile_definitions(RUSH_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(RUSH_LINUX)
else()
    add_compile_definitions(RUSH_APPLE)  
endif()

target_link_libraries(Rudder PUBLIC Rush)