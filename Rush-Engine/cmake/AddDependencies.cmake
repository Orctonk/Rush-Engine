# --------------- Set options ---------------

# nlohmann_json options
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(BUILD_TESTING OFF CACHE INTERNAL "")

# cereal options
set(SKIP_PERFORMANCE_COMPARISON ON CACHE INTERNAL "")
set(BUILD_DOC OFF CACHE INTERNAL "")
set(BUILD_SANDBOX OFF CACHE INTERNAL "")

# Shaderc options
set(ENABLE_OPT OFF CACHE INTERNAL "")
set(ENABLE_GLSLANG_BINARIES OFF CACHE INTERNAL "")
set(BUILD_EXTERNAL OFF CACHE INTERNAL "")
set(SKIP_GLSLANG_INSTALL ON CACHE INTERNAL "")
set(SKIP_SPIRV_INSTALL ON CACHE INTERNAL "")
set(SHADERC_SKIP_COPYRIGHT_CHECK ON CACHE INTERNAL "")
set(SPIRV_TOOLS_BUILD_STATIC (NOT ${RUSH_BUILD_SHARED}) CACHE INTERNAL "")
set(SPIRV_HEADERS_SKIP_EXAMPLES ON CACHE INTERNAL "")
set(SPIRV_HEADERS_SKIP_INSTALL ON CACHE INTERNAL "")
set(SPIRV_SKIP_EXECUTABLES ON CACHE INTERNAL "")
set(SPIRV_SKIP_TESTS ON CACHE INTERNAL "")
set(SHADERC_SKIP_INSTALL ON CACHE INTERNAL "")
set(SHADERC_SKIP_TESTS ON CACHE INTERNAL "")
set(SHADERC_SKIP_EXAMPLES ON CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

# SPIRV-cross options
set(SPIRV_CROSS_SHARED ${RUSH_BUILD_SHARED} CACHE INTERNAL "")
set(SPIRV_CROSS_STATIC (NOT ${RUSH_BUILD_SHARED}) CACHE INTERNAL "")
set(SPIRV_CROSS_CLI OFF CACHE INTERNAL "")
set(SPIRV_CROSS_ENABLE_TESTS OFF CACHE INTERNAL "")
set(SPIRV_CROSS_ENABLE_HLSL OFF CACHE INTERNAL "")
set(SPIRV_CROSS_ENABLE_MSL OFF CACHE INTERNAL "")
set(SPIRV_CROSS_ENABLE_CPP OFF CACHE INTERNAL "")
set(SPIRV_CROSS_ENABLE_C_API OFF CACHE INTERNAL "")
set(SPIRV_CROSS_ENABLE_REFLECT OFF CACHE INTERNAL "")
set(SPIRV_CROSS_ENABLE_UTIL OFF CACHE INTERNAL "")
set(SPIRV_CROSS_SKIP_INSTALL ON CACHE INTERNAL "")

# Assimp options
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# --------------- Fetch the dependencies ---------------
include(FetchContent)

function(addDependency depname repo tag)
    FetchContent_Declare(${depname}
        GIT_REPOSITORY ${repo}
        GIT_TAG        ${tag}) # Version 0.9.9.8
    FetchContent_MakeAvailable(${depname})
    set(${depname}_SOURCE_DIR ${${depname}_SOURCE_DIR} PARENT_SCOPE)
endfunction(addDependency depname repo tag)

# Add GLM Version 0.9.9.8 as dependency
addDependency(glm "https://github.com/g-truc/glm.git" 0.9.9.8)
target_link_libraries(Rush PUBLIC glm::glm)

# Add stb_image as dependency
addDependency(stb "https://github.com/nothings/stb" af1a5bc352164740c1cc1354942b1c6b72eacb8a) # Master branch on 2022-04-13
target_include_directories(Rush PRIVATE ${stb_SOURCE_DIR})

# Add ImGui as dependency
addDependency(imgui "https://github.com/ocornut/imgui.git" 505f19e3b00d54623566dda354980c7f3410f2b5) # Docking branch on 2022-04-13
target_include_directories(Rush PUBLIC ${imgui_SOURCE_DIR})

# Add fmt as a dependency
addDependency(fmt "https://github.com/fmtlib/fmt" 8.1.1) # Version 8.1.1
target_link_libraries(Rush PUBLIC fmt::fmt)

# Add nlohmann/json parser as dependency
addDependency(nlohmann_json "https://github.com/nlohmann/json" v3.10.5) # Version 3.10.5
target_link_libraries(Rush PRIVATE nlohmann_json::nlohmann_json)

# Add Cereal as dependency for serialization
addDependency(cereal "https://github.com/USCiLab/cereal" v1.3.2) # Version 1.3.2
target_link_libraries(Rush PUBLIC cereal::cereal)

# Add shaderc as dependency for shader compilation into SPIR-V
addDependency(glslang "https://github.com/KhronosGroup/glslang" abbe466451ca975fecfdba453ef9073df52aefc5) # Version 11.9.0
addDependency(spirvheaders  "https://github.com/KhronosGroup/SPIRV-Headers" 9c3fd01c8a91761b6e4be97ff4f13de76e779128) # Version sdk-1.3.204.1
addDependency(sprivtools  "https://github.com/KhronosGroup/SPIRV-tools" eed5c76a57bb965f2e1b56d1dc40b50910b5ec1d) # Version 2022.2
set(SHADERC_SPIRV_TOOLS_DIR ${sprivtools_SOURCE_DIR} CACHE INTERNAL "")
set(SHADERC_SPIRV_HEADERS_DIR ${sprivheaders_SOURCE_DIR} CACHE INTERNAL "")
set(SHADERC_GLSLANG_DIR ${glslang_SOURCE_DIR} CACHE INTERNAL "")
addDependency(shaderc  "https://github.com/google/shaderc" 1bbf43f210941ba69a2cd05cf3529063f1ff5bb9) # Version 2022.1
target_link_libraries(Rush PUBLIC shaderc)

# Add SPIRV-cross as a dependency for cross compilation from SPIRV into other shader formats
addDependency(spirvcross "https://github.com/KhronosGroup/SPIRV-Cross" sdk-1.3.204.1) # Version sdk-1.3.204.1
target_include_directories(Rush PRIVATE ${spirvcross_SOURCE_DIR})
target_link_libraries(Rush PRIVATE spirv-cross-core)
target_link_libraries(Rush PRIVATE spirv-cross-glsl)

# Build or add Assimp library as dependency
if(RUSH_BUILD_ASSIMP)
    message(STATUS "Building assimp")
    addDependency(assimp "https://github.com/assimp/assimp" v5.2.3) # Version 5.2.3
    target_link_libraries(Rush PRIVATE assimp::assimp)
else()
    find_package(Assimp REQUIRED)
    target_include_directories(Rush PRIVATE ${ASSIMP_INCLUDE_DIRS})
    target_link_libraries(Rush PRIVATE assimp::assimp)
endif()

# Add EnTT as dependency
addDependency(entt "https://github.com/skypjack/entt" v3.9.0) # Version 3.9.0
target_link_libraries(Rush PUBLIC EnTT::EnTT)
if(${RUSH_BUILD_SHARED})
    target_compile_definitions (Rush PRIVATE ENTT_API_EXPORT)
    target_compile_definitions (Rush INTERFACE ENTT_API_IMPORT)
endif()

# Add Mono as dependency
find_package(Mono REQUIRED)
message("Mono found at ${MONO_ROOT}")
target_compile_options(Rush PUBLIC ${MONO_CFLAGS})
target_link_directories(Rush PUBLIC ${MONO_LIBRARY_DIRS})
target_compile_definitions(Rush PRIVATE "RUSH_MONO_LIB_DIR=\"${MONO_LIBRARY_DIRS}\"")
target_compile_definitions(Rush PRIVATE "RUSH_MONO_CONFIG_DIR=\"${MONO_ROOT}/etc/mono/\"")
target_link_libraries(Rush PUBLIC ${MONO_LIBRARIES})
target_include_directories(Rush PRIVATE ${MONO_INCLUDE_DIRS})

# Add Window manager and renderer implementation as dependency
if(RUSH_OPENGL)
    add_compile_definitions(RUSH_OPENGL)
    addDependency(glfw "https://github.com/glfw/glfw" 3.3.7) # Version 3.3.7
    target_link_libraries(Rush PRIVATE glfw)

    find_package(OpenGL REQUIRED)
    target_link_libraries(Rush PRIVATE OpenGL::GL)

    add_subdirectory(${RUSH_DEPS_DIR}/glad)
    target_link_libraries(Rush PRIVATE glad)
endif()

add_custom_command(TARGET Rush POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/mono/mcs.exe
    $<TARGET_FILE_DIR:Rush>/mcs.exe)

target_compile_definitions(Rush PRIVATE "RUSH_BINDINGS_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/src/Rush/scripting/csbindings/\"")
target_compile_definitions(Rush PRIVATE "RUSH_MCS_EXEC=\"${CMAKE_CURRENT_SOURCE_DIR}/Libraries/mono/mcs.exe\"")
addDependency(glmsharp "https://github.com/Philip-Trettner/GlmSharp" f6a1a766437159ab1850c80e818768e37d161dfc) # Master branch on 2022-04-13
target_compile_definitions(Rush PRIVATE "RUSH_GLM_BINDINGS_DIR=\"${glmsharp_SOURCE_DIR}/GlmSharp/\"")