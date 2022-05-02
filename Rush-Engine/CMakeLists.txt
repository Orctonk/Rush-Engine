cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Rush options
option(RUSH_OPENGL "Use OpenGL for rendering" ON)
option(RUSH_BUILD_SHARED "Build Rush as a shared library" ON)
option(RUSH_STATIC_CRT "Use static CRT with Rush" OFF)
option(RUSH_BUILD_ASSIMP "Build Assimp from source" ON)

project(Rush VERSION 1.0)

# Define directory structure
set(RUSH_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(RUSH_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RUSH_DEPS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries")
set(RUSH_CMAKE_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake-config")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/$<CONFIG>/lib")

# ----------------------------------------------
# -------------- Set source files --------------
# ----------------------------------------------

file(GLOB_RECURSE RUSH_HEADERS CONFIGURE_DEPENDS "${RUSH_SOURCE_DIR}/Rush/*.h")
list(APPEND RUSH_HEADERS "${RUSH_SOURCE_DIR}/Rush.h" "${RUSH_SOURCE_DIR}/Rushpch.h")

# Set Rush implementation independent sources
file(GLOB_RECURSE RUSH_SOURCES CONFIGURE_DEPENDS "${RUSH_SOURCE_DIR}/Rush/*.cpp")

# Add implementation specific sources
if(RUSH_OPENGL)
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS "${RUSH_SOURCE_DIR}/Platform/OpenGL/*.cpp")
    file(GLOB_RECURSE PLATFORM_HEADERS CONFIGURE_DEPENDS "${RUSH_SOURCE_DIR}/Platform/OpenGL/*.h")
else()
    message(FATAL_ERROR "No window manager provided!")
endif()

list(APPEND RUSH_SOURCES ${PLATFORM_SOURCES})
list(APPEND RUSH_HEADERS ${PLATFORM_HEADERS})

# -------------------------------------------------------
# -------------- Set library build options --------------
# -------------------------------------------------------

# Specify library type
if(RUSH_BUILD_SHARED)
    add_library(Rush SHARED ${RUSH_SOURCES} ${RUSH_HEADERS})
    target_compile_definitions(Rush PUBLIC RUSH_SHARED)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
else()
    add_library(Rush STATIC ${RUSH_SOURCES} ${RUSH_HEADERS})
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
endif()
target_compile_definitions(Rush PRIVATE RUSH_BUILD)

# Configure RushVersion file for use in applications
configure_file(${RUSH_CMAKE_CONFIG_DIR}/RushVersion.h.in RushVersion.h)

# Add precompiled header
target_precompile_headers(Rush PRIVATE ${RUSH_SOURCE_DIR}/Rushpch.h)

# Set public includes for Rush
target_include_directories(Rush PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(Rush PUBLIC ${RUSH_SOURCE_DIR})

# Set Rush debug flag
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DRUSH_DEBUG")

# Ensure C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Add platform dependent libraries
if(WIN32)
    add_definitions(-DRUSH_WINDOWS)
	if(RUSH_STATIC_CRT)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
	endif()
elseif(UNIX AND NOT APPLE)
    target_link_libraries(Rush PRIVATE pthread)
    add_definitions(-DRUSH_LINUX)
else()
    message(FATAL_ERROR "OSX is not yet supported")
endif()

# ------------------------------------------
# -------------- Dependencies --------------
# ------------------------------------------

# Set CRT options for dependencies
if(RUSH_STATIC_CRT)
    set(RUSH_SHARED_CRT OFF)
else(RUSH_STATIC_CRT)
    set(RUSH_SHARED_CRT ON)
endif(RUSH_STATIC_CRT)

set(OVERRIDE_MSVCCRT OFF CACHE INTERNAL "")
set(SHADERC_ENABLE_SHARED_CRT ${RUSH_SHARED_CRT} CACHE INTERNAL "")
set(USE_STATIC_CRT ${RUSH_STATIC_CRT} CACHE INTERNAL "")

# Include Dependencies

include(cmake/AddDependencies.cmake)

install(TARGETS Rush
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)