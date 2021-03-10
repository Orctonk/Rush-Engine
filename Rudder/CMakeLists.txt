cmake_minimum_required(VERSION 3.17)

project(Rudder VERSION 1.0)

set(RUDDER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RUDDER_DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")

set(RUDDER_SOURCES 
    ${RUDDER_SOURCE_DIR}/EditorApplication.cpp
    ${RUDDER_SOURCE_DIR}/EditorLayer.cpp
    ${RUDDER_SOURCE_DIR}/GlobalEntitySelection.cpp
    ${RUDDER_SOURCE_DIR}/widgets/EntityEditorWidget.cpp
    ${RUDDER_SOURCE_DIR}/widgets/FileBrowser.cpp
    ${RUDDER_SOURCE_DIR}/views/RenderViews.cpp
    ${RUDDER_SOURCE_DIR}/views/CameraController.cpp
    ${RUDDER_SOURCE_DIR}/views/SceneGraphView.cpp
    ${RUDDER_SOURCE_DIR}/views/SceneView.cpp
    ${RUDDER_SOURCE_DIR}/views/AssetView.cpp
    ${RUDDER_SOURCE_DIR}/views/ProfilerResultView.cpp
)

list(APPEND RUDDER_SOURCES
    ${RUDDER_DEPS_DIR}/ImGuizmo/ImGuizmo.cpp
)

add_executable(Rudder ${RUDDER_SOURCES})

# Add precompiled header
target_precompile_headers(Rudder PRIVATE ${RUDDER_SOURCE_DIR}/Rudderpch.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

target_include_directories(Rudder PRIVATE ${RUDDER_SOURCE_DIR})
target_include_directories(Rudder PRIVATE ${RUDDER_DEPS_DIR})

if(WINDOWS)
    add_compile_definitions(RUSH_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(RUSH_LINUX)
else()
    add_compile_definitions(RUSH_APPLE)  
endif()

target_link_libraries(Rudder PRIVATE Rush)

add_custom_command(TARGET Rudder POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res 
    ${CMAKE_CURRENT_BINARY_DIR}/res)

add_custom_command(TARGET Rudder POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
    ${CMAKE_CURRENT_BINARY_DIR}/imgui.ini)

add_custom_command(TARGET Rudder POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:Rush>
    $<TARGET_FILE_DIR:Rudder>)

install(TARGETS Rudder
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)
