#ifndef __SCRIPTINGBACKEND_H__
#define __SCRIPTINGBACKEND_H__

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/metadata/debug-helpers.h>
#include <Rush/core/Core.h>
#include <string>
#include <vector>
#include "BindingBase.h"

namespace Rush {

class ScriptingBackend {
    static MonoDomain *s_RootDomain;
    static MonoDomain *s_AppDomain;
    static MonoAssembly *s_Assembly;
    static MonoImage *s_Image;

    static std::vector<Unique<Bindings::BindingBase>> s_Bindings;

    static std::string CompileFiles(std::vector<std::string> filenames, std::string outName);
    static void AddBindings();
public:
    ScriptingBackend() = delete;

    static const std::string BindingsBaseDir;

    static void Init();
    static void LoadAssemblies(std::vector<std::string> extra = {});
    static void UnloadAssemblies();
    static void Shutdown();

    static void InvokeInstanceVoid(MonoObject *instance, std::string method);

    static MonoDomain *GetRootDomain() { return s_RootDomain; }
    static MonoDomain *GetAppDomain() { return s_AppDomain; }
    static MonoAssembly *GetAssembly() { return s_Assembly; }
    static MonoImage *GetImage() { return s_Image; }
};

}

#endif // __SCRIPTINGBACKEND_H__
