cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Rush options
option(RUSH_OPENGL "Use OpenGL for rendering" ON)
option(RUSH_BUILD_SHARED "Build Rush as a shared library" OFF)
option(RUSH_BUILD_ASSIMP "Build Assimp from source" OFF)

project(Rush VERSION 1.0)

# Define directory structure
set(RUSH_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(RUSH_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RUSH_DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")
set(RUSH_CMAKE_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake-config")

# Set Rush implementation independent sources
set(RUSH_SOURCES    ${RUSH_SOURCE_DIR}/Rush/core/Application.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/Logger.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/AbstractWindow.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/Layer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/LayerStack.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/Time.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/Input.cpp
                    ${RUSH_SOURCE_DIR}/Rush/core/Random.cpp
                    ${RUSH_SOURCE_DIR}/Rush/events/EventQueue.cpp
                    ${RUSH_SOURCE_DIR}/Rush/profiling/Profiler.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Shader.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Texture.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Cubemap.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/VertexArray.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/VertexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/IndexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/IndexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Renderer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/LineRenderer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Renderer2D.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/RenderingAPI.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/Framebuffer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/graphics/particles/ParticleSystem.cpp
                    ${RUSH_SOURCE_DIR}/Rush/scene/Camera.cpp
                    ${RUSH_SOURCE_DIR}/Rush/scene/Scene.cpp
                    ${RUSH_SOURCE_DIR}/Rush/resources/AssetManager.cpp
                    ${RUSH_SOURCE_DIR}/Rush/resources/ModelLoader.cpp
                    ${RUSH_SOURCE_DIR}/Rush/resources/Path.cpp
                    ${RUSH_SOURCE_DIR}/Rush/resources/File.cpp
                    ${RUSH_SOURCE_DIR}/Rush/resources/Material.cpp
                    ${RUSH_SOURCE_DIR}/Rush/imgui/ImguiLayer.cpp
                    ${RUSH_SOURCE_DIR}/Rush/imgui/ImguiImpl.cpp
                    ${RUSH_SOURCE_DIR}/Rush/debug/DebugLayer.cpp
                    )

# Add implementation specific sources
if(RUSH_OPENGL)
    list(APPEND RUSH_SOURCES 
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/GLFWWindow.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/GLFWContext.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/ShaderDataUtil.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLShader.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLTexture.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLCubemap.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLVertexArray.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLVertexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLIndexBuffer.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLRenderingAPI.cpp
                    ${RUSH_SOURCE_DIR}/Platform/OpenGL/OGLFramebuffer.cpp
                    )
else()
    message(FATAL_ERROR "No window manager provided!")
endif()

# Specify library type
if(RUSH_BUILD_SHARED)
    add_library(Rush SHARED ${RUSH_SOURCES})
else()
    add_library(Rush STATIC ${RUSH_SOURCES})
endif()

# Configure RushVersion file for use in applications
configure_file(${RUSH_CMAKE_CONFIG_DIR}/RushVersion.h.in RushVersion.h)

# Add precompiled header
target_precompile_headers(Rush PRIVATE ${RUSH_SOURCE_DIR}/Rushpch.h)

# Set public includes for rush
target_include_directories(Rush PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(Rush PUBLIC ${RUSH_SOURCE_DIR})

# Set Rush debug flag
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DRUSH_DEBUG")

# Ensure C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Add platform dependent libraries
if(WINDOWS)
    message(WARNING "Windows has not yet been tested!")
    add_definitions(-DRUSH_WINDOWS)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(Rush PRIVATE pthread)
    add_definitions(-DRUSH_LINUX)
else()
    message(FATAL_ERROR "OSX is not yet supported")
endif()

# Add GLM as dependency
set(glm_DIR "${RUSH_DEPS_DIR}/glm/cmake/glm")
find_package(glm REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIRS}")
target_link_libraries(Rush PUBLIC glm::glm)
target_include_directories(Rush PRIVATE ${GLM_INCLUDE_DIRS})

# Add stb_image as dependency
target_include_directories(Rush PRIVATE ${RUSH_DEPS_DIR}/stb_image)

# Add ImGui as dependency
target_include_directories(Rush PUBLIC ${RUSH_DEPS_DIR}/imgui)

# Add nlohmann/json parser as dependency
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(BUILD_TESTING OFF CACHE INTERNAL "")
add_subdirectory(${RUSH_DEPS_DIR}/Json)
target_link_libraries(Rush PRIVATE nlohmann_json::nlohmann_json)

# Build or add Assimp library as dependency
if(RUSH_BUILD_ASSIMP)
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)
    add_subdirectory("${RUSH_DEPS_DIR}/assimp")
    target_link_libraries(Rush PRIVATE assimp)
    message(STATUS "Building assimp")
else()
    find_package(Assimp REQUIRED)
    target_include_directories(Rush PRIVATE ${ASSIMP_INCLUDE_DIRS})
    target_link_libraries(Rush PRIVATE ${ASSIMP_LIBRARIES})
endif()

# Add EnTT as dependency
add_subdirectory("${RUSH_DEPS_DIR}/entt")
target_link_libraries(Rush PUBLIC EnTT::EnTT)

# Add Window manager and renderer implementation as dependency
if(RUSH_OPENGL)
    add_compile_definitions(RUSH_OPENGL)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    if(RUSH_BUILD_SHARED)
        set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
    endif()
    add_subdirectory("${RUSH_DEPS_DIR}/glfw")
    target_link_libraries(Rush PRIVATE glfw)

    find_package(OpenGL REQUIRED)
    target_link_libraries(Rush PRIVATE OpenGL::GL)

    add_subdirectory(${RUSH_DEPS_DIR}/glad)
    target_link_libraries(Rush PRIVATE glad)
endif()