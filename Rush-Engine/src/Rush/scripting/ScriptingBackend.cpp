#include "ScriptingBackend.h"
#include <iostream>

#include "Behaviour.binding.h"
#include "Entity.binding.h"
#include "modules/GLM.binding.h"
#include "modules/Input.binding.h"
#include "modules/Logger.binding.h"
#include "modules/Random.binding.h"
#include "modules/Time.binding.h"
// #include "components/LightComponent.binding.h"
#include "components/TransformComponent.binding.h"

#include <mono/metadata/exception.h>
#include <mono/metadata/mono-config.h>

#define REGISTER_BINDING(binding) s_Bindings.push_back(CreateUnique<binding>())

namespace Rush {

MonoDomain *ScriptingBackend::s_RootDomain = nullptr;
MonoDomain *ScriptingBackend::s_AppDomain = nullptr;
MonoAssembly *ScriptingBackend::s_Assembly = nullptr;
MonoImage *ScriptingBackend::s_Image = nullptr;
const std::string ScriptingBackend::BindingsBaseDir = RUSH_BINDINGS_DIR;
std::vector<Unique<Bindings::BindingBase>> ScriptingBackend::s_Bindings;

struct ExecRes {
    std::string output;
    int code;
};

ExecRes exec(std::string cmd) {
    cmd += " 1> compilation_output.txt 2>&1";
    ExecRes res;
    res.code = system(cmd.c_str());

    char buffer[512];
    std::ifstream outFile("compilation_output.txt");
    while (!outFile.getline(buffer, 512).eof())
        res.output = res.output + buffer + "\n";
    return res;
}

std::string ScriptingBackend::CompileFiles(std::vector<std::string> filenames, std::string outName) {
    std::ofstream respFile("scripting_sources.resp", std::ifstream::trunc);
    respFile << "-r:System.Numerics.dll" << std::endl;
    respFile << "-r:System.Runtime.Serialization.dll" << std::endl;
    respFile << "-t:library" << std::endl;
    respFile << "-out:" << outName << std::endl;
    for (std::string &file : filenames)
        respFile << "\"" << file << "\"\n";

    respFile.close();

    std::string command = "mcs @\"scripting_sources.resp\"";

    RUSH_LOG_SCOPE_ALIAS("C# Compiler");
    RUSH_LOG_INFO("Running command: {0}", command);
    ExecRes res = exec(command);
    size_t pos = 0;
    size_t prev = 0;
    while ((pos = res.output.find('\n', pos + 1)) != std::string::npos) {
        if (res.output.find("error", prev) == prev)
            RUSH_LOG_ERROR(res.output.substr(prev + 6, pos - (prev + 6)));
        else if (res.output.find("Compilation failed", prev))
            RUSH_LOG_ERROR(res.output.substr(prev, pos - prev));
        else
            RUSH_LOG_INFO(res.output.substr(prev, pos - prev));
        prev = pos + 1;
    }

    if (res.code != 0)
        RUSH_LOG_ERROR("Failed to compile file '{0}'", outName);
    else
        RUSH_LOG_INFO("Compiled '{0}'", outName);
    return outName;
}

void ScriptingBackend::Init() {
    RUSH_LOG_SCOPE_ALIAS("ScriptingBackend");
    RUSH_LOG_INFO("Initializing...");
    RUSH_LOG_INFO("Adding Bindings");
    AddBindings();

    if (!s_RootDomain) {
        RUSH_LOG_INFO("Creating mono domain");
        mono_set_dirs(RUSH_MONO_LIB_DIR, RUSH_MONO_CONFIG_DIR);
        mono_config_parse(NULL);
        s_RootDomain = mono_jit_init_version("Scripting Backend", "v4.0.30319");
        if (s_RootDomain) {
            RUSH_LOG_INFO("Mono runtime version {0}", mono_get_runtime_build_info());
            // Bind all methods
            for (auto &b : s_Bindings)
                b->BindMethods();
        }
    } else {
        RUSH_LOG_WARNING("Scripting backend has already been initialized");
    }
}
void ScriptingBackend::LoadAssemblies(std::vector<std::string> extra) {
    RUSH_LOG_SCOPE_ALIAS("ScriptingBackend");
    if (s_RootDomain) {
        if (s_Assembly) {
            RUSH_LOG_WARNING("Another assembly was already loaded, unloading...");
            UnloadAssemblies();
        }
        // Get file list
        std::vector<std::string> files;
        for (auto &b : s_Bindings) {
            std::vector<std::string> bFiles = b->GetFilenames();
            files.insert(files.end(), bFiles.begin(), bFiles.end());
        }
        std::string filename = CompileFiles(files, "scripting.dll");
        s_AppDomain = mono_domain_create_appdomain("Scripting", NULL);
        mono_domain_set(s_AppDomain, false);
        s_Assembly = mono_domain_assembly_open(s_AppDomain, filename.c_str());
        if (s_Assembly) {
            RUSH_LOG_INFO("Assembly Loaded");
            s_Image = mono_assembly_get_image(s_Assembly);
            // Initialize modules
            for (auto &b : s_Bindings)
                b->Init();
        } else {
            RUSH_LOG_ERROR("Failed to load assembly");
        }
    } else
        RUSH_LOG_ERROR("Mono domain has not been initialized yet");
}

void ScriptingBackend::UnloadAssemblies() {
    RUSH_LOG_SCOPE_ALIAS("ScriptingBackend");
    if (s_Assembly != nullptr) {
        if (s_AppDomain) {
            mono_domain_set(s_RootDomain, false);
            mono_domain_unload(s_AppDomain);
        }
        s_AppDomain = nullptr;
        s_Assembly = nullptr;
        RUSH_LOG_INFO("Unloaded assembly");
    } else {
        RUSH_LOG_WARNING("No assembly loaded");
    }
}

void ScriptingBackend::Shutdown() {
    mono_jit_cleanup(s_RootDomain);
}

void ScriptingBackend::InvokeInstanceVoid(MonoObject *instance, std::string method) {
    MonoClass *c = mono_object_get_class(instance);
    MonoMethod *m = mono_class_get_method_from_name(c, method.c_str(), 0);
    // TODO: Implement error handling
    mono_runtime_invoke(m, instance, NULL, NULL);
}

void ScriptingBackend::AddBindings() {
    REGISTER_BINDING(Bindings::Behaviour);
    REGISTER_BINDING(Bindings::Component);
    REGISTER_BINDING(Bindings::Entity);
    // REGISTER_BINDING(Bindings::LightComponent);
    REGISTER_BINDING(Bindings::TransformComponent);

    REGISTER_BINDING(Bindings::GLM);
    REGISTER_BINDING(Bindings::Input);
    REGISTER_BINDING(Bindings::Logger);
    REGISTER_BINDING(Bindings::Random);
    REGISTER_BINDING(Bindings::Time);
}

} // namespace Rush
