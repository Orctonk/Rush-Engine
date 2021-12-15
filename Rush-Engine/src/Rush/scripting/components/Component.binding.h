#ifndef __COMPONENT_BINDING_H__
#define __COMPONENT_BINDING_H__

#include "../BindingBase.h"
#include "../ScriptingBackend.h"
#include "Rush/scene/Entity.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/object.h>

namespace Rush {
namespace Bindings {

class Component : public BindingBase {
public:
    virtual FileList GetFilenames() override { return { ScriptingBackend::BindingsBaseDir + "components/Component.cs" }; }
    virtual void Init() override;
    virtual void BindMethods() override;

protected:
    static void *GetHandle(MonoObject *instance);
    static MonoObject *CreateComponent(Rush::Entity parent, void *nativePtr, const char *typeName);
};

#define COMPONENT_BODY(name, path)                                             \
    virtual FileList GetFilenames() override { return { path }; }              \
    virtual void Init() override;                                              \
    virtual void BindMethods() override;                                       \
    static MonoObject *CreateComponent(Rush::Entity parent, void *nativePtr) { \
        return Component::CreateComponent(parent, nativePtr, #name);           \
    }

} // namespace Bindings
} // namespace Rush

#endif // __COMPONENT_BINDING_H__
