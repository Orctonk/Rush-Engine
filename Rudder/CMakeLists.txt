cmake_minimum_required(VERSION 3.10)

project(Rudder VERSION 1.0)

set(RUDDER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RUDDER_DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")

set(RUDDER_SOURCES 
        ${RUDDER_SOURCE_DIR}/EditorApplication.cpp
        ${RUDDER_SOURCE_DIR}/EditorLayer.cpp
        ${RUDDER_SOURCE_DIR}/widgets/EntityEditorWidget.cpp
        ${RUDDER_SOURCE_DIR}/widgets/FileBrowser.cpp
        ${RUDDER_SOURCE_DIR}/views/RenderViews.cpp
        ${RUDDER_SOURCE_DIR}/views/CameraController.cpp
        ${RUDDER_SOURCE_DIR}/views/SceneGraphView.cpp
        ${RUDDER_SOURCE_DIR}/views/AssetView.cpp
    )

add_executable(Rudder ${RUDDER_SOURCES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(COPY res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(Rudder PRIVATE ${RUDDER_SOURCE_DIR})

if(WINDOWS)
    add_compile_definitions(RUSH_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_compile_definitions(RUSH_LINUX)
else()
    add_compile_definitions(RUSH_APPLE)  
endif()

target_link_libraries(Rudder PUBLIC Rush)