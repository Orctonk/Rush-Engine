cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

option(RUSH_OPENGL "Use OpenGL for rendering" ON)
option(RUSH_BUILD_SHARED "Build Rush as a shared library" OFF)
option(RUSH_BUILD_ASSIMP "Build Assimp from source" OFF)

project(Rush VERSION 1.0)

set(RUSH_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(RUSH_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RUSH_DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")
set(RUSH_CMAKE_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake-config")
set(RUSH_OUT_DIR "out")

set(RUSH_SOURCES    ${RUSH_SOURCE_DIR}/Rush/core/Application.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/Logger.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/AbstractWindow.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Shader.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Texture.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/VertexArray.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/VertexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/IndexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/IndexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Renderer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/RenderingAPI.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/Camera.cpp)

if(RUSH_OPENGL)
    list(APPEND RUSH_SOURCES 
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/GLFWWindow.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/GLFWContext.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/ShaderDataUtil.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLShader.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLTexture.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLVertexArray.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLVertexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLIndexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLRenderingAPI.cpp)
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
target_include_directories(Rush PUBLIC ${RUSH_SOURCE_DIR})
target_precompile_headers(Rush PRIVATE ${RUSH_SOURCE_DIR}/Rushpch.h)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DRUSH_DEBUG")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if(WINDOWS)
    message(WARNING "Windows has not yet been tested!")
elseif(UNIX AND NOT APPLE)
    target_link_libraries(Rush pthread)
else()
    message(FATAL_ERROR "OSX is not yet supported")
endif()

set(glm_DIR "${RUSH_DEPS_DIR}/glm/cmake/glm")
find_package(glm REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIRS}")
target_link_libraries(Rush glm::glm)
target_include_directories(Rush PRIVATE ${GLM_INCLUDE_DIRS})

target_include_directories(Rush PRIVATE ${RUSH_DEPS_DIR}/stb_image)

if(RUSH_BUILD_ASSIMP)
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)
    add_subdirectory("${RUSH_DEPS_DIR}/assimp")
    target_link_libraries(Rush assimp)
    message(STATUS "Building assimp")
else()
    find_package(Assimp REQUIRED)
    target_include_directories(Rush PRIVATE ${ASSIMP_INCLUDE_DIRS})
    target_link_libraries(Rush ${ASSIMP_LIBRARIES})
endif()

if(RUSH_OPENGL)
    add_compile_definitions(RUSH_OPENGL)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    if(RUSH_BUILD_SHARED)
        set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
    endif()
    add_subdirectory("${RUSH_DEPS_DIR}/glfw")
    target_link_libraries(Rush glfw)

    find_package(OpenGL REQUIRED)
    target_link_libraries(Rush OpenGL::GL)

    add_subdirectory(${RUSH_DEPS_DIR}/glad)
    target_link_libraries(Rush glad)
endif()