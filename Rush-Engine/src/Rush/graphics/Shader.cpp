#include "Rushpch.h"
#include "Shader.h"
#include "Rush/resources/File.h"
#include "Rush/resources/FileCacher.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLShader.h"
#else
    #error "No rendering API provided!"
#endif

#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>

namespace Rush {

Shader::Shader(std::string shaderPath) : m_DebugName(shaderPath) { 
    SourceMap sources = LoadSource(shaderPath);
    CompileSPIRV(sources);
    Reflect();
}

Shader::~Shader() { }

Shared<Shader> Shader::Create(Path shaderPath) {
    return CreateShared<OGLShader>(shaderPath);
}

Shader::SourceMap Shader::LoadSource(Path shaderPath) {
    SourceMap sources;

    m_SourceModTime = std::filesystem::last_write_time(shaderPath.GetRawPath());

    File shaderFile(shaderPath);
    if (!shaderFile.Exists() || shaderFile.GetType() != FileType::Regular) {
        RUSH_LOG_WARNING("Could not open file '{}'!", shaderPath.GetRawPath());
        return sources;
    }
    std::fstream fs = shaderFile.OpenFile(OpenMode::Read);
    std::string line;
    ShaderType currentType = ShaderType::None;
    while (!fs.eof()) {
        std::stringstream ss;
        getline(fs, line);
        while (!fs.eof() && line.substr(0, 5) != "#type") {
            if (currentType == ShaderType::None)
                RUSH_LOG_WARNING("No '#type' read! Discarding shader line '{}'", line);
            else
                ss << line << "\n";
            getline(fs, line);
        }
        if (fs.eof())
            ss << line << "\n";

        if (currentType != ShaderType::None)
            sources.emplace(currentType,ss.str());

        if (line.substr(0, 5) == "#type") {
            if (line.substr(6, 6) == "vertex")
                currentType = ShaderType::Vertex;
            else if (line.substr(6, 8) == "geometry")
                currentType = ShaderType::Geometry;
            else if (line.substr(6, 8) == "fragment")
                currentType = ShaderType::Fragment;
            else
                RUSH_ASSERT(false);
        }
    }
    return sources;
}

void Shader::CompileSPIRV(SourceMap sources) {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    //options.SetOptimizationLevel(shaderc_optimization_level_performance);

    m_SPIRVBinaries.clear();

    for (auto&& [type, source] : sources) {
        Path cachedBinaryPath = Path(m_DebugName).GetFileName() + ".vk." + TypeToString(type);
        if(!FileCache::GetCachedBinaryFile(cachedBinaryPath.GetFullFileName(),m_SPIRVBinaries[type],m_SourceModTime)){
            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, TypeToShaderc(type), m_DebugName.c_str(), options);
            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                RUSH_LOG_ERROR(module.GetErrorMessage());
                RUSH_ASSERT(false);
            }

            m_SPIRVBinaries[type] = std::vector<uint32_t>(module.cbegin(), module.cend());

            FileCache::CacheBinaryFile(cachedBinaryPath.GetFullFileName(),m_SPIRVBinaries[type]);
        }
    }
}

void Shader::Reflect() {
    for (auto&& [type, binary] : m_SPIRVBinaries) {
        spirv_cross::Compiler compiler(binary);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();
        RUSH_LOG_INFO("Shader reflection '{}:{}'",m_DebugName,TypeToString(type));
        RUSH_LOG_INFO("  {} Uniform buffers", resources.uniform_buffers.size());
        for (const auto& resource : resources.uniform_buffers) {
            const auto& bufferType = compiler.get_type(resource.base_type_id);
            uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            int memberCount = bufferType.member_types.size();

            RUSH_LOG_INFO("    {}", resource.name);
            RUSH_LOG_INFO("      Size = {}", bufferSize);
            RUSH_LOG_INFO("      Binding = {}", binding);
            RUSH_LOG_INFO("      Members = {}", memberCount);
        }
        RUSH_LOG_INFO("  {} images", resources.sampled_images.size());
    }
}

shaderc_shader_kind Shader::TypeToShaderc(ShaderType type) {
    switch (type)
    {
    case Rush::ShaderType::Vertex:
        return shaderc_vertex_shader;
    case Rush::ShaderType::Geometry:
        return shaderc_geometry_shader;
    case Rush::ShaderType::Fragment:
        return shaderc_fragment_shader;
    default:
        RUSH_ASSERT(false);
        return (shaderc_shader_kind)0;
    }
}

std::string Shader::TypeToString(ShaderType type) {
    switch (type)
    {
    case Rush::ShaderType::Vertex:
        return "Vertex";
    case Rush::ShaderType::Geometry:
        return "Geometry";
    case Rush::ShaderType::Fragment:
        return "Fragment";
    default:
        return "None";
    }
}

}