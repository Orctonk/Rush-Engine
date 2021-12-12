#ifndef __ENTITY_BINDING_H__
#define __ENTITY_BINDING_H__

#include "BindingBase.h"
#include "ScriptingBackend.h"

#include <Rush/scene/Entity.h>
#include <mono/metadata/object.h>

namespace Rush {
namespace Bindings {

class Entity : public BindingBase {
private:
    static MonoObject *GetLightComponent(MonoObject *instance);
    static Rush::Entity EntityFromMonoInstance(MonoObject *instance);

public:
    virtual void Init() override;
    virtual std::string GetFilename() override { return ScriptingBackend::BindingsBaseDir + "Entity.cs"; }
    virtual void BindMethods() override;

    static MonoObject *CreateEntity(Rush::Entity entity);
};

} // namespace Bindings
} // namespace Rush

#endif // __ENTITY_BINDING_H__
