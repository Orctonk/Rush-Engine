#ifndef __ENTITY_BINDING_H__
#define __ENTITY_BINDING_H__

#include "BindingBase.h"
#include "ScriptingBackend.h"

#include <mono/metadata/object.h>
#include <Rush/scene/Entity.h>

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

}
}

#endif // __ENTITY_BINDING_H__
