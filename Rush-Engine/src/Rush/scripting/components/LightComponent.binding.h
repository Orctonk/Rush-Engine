#ifndef __LIGHTCOMPONENT_BINDING_H__
#define __LIGHTCOMPONENT_BINDING_H__

#include "Component.binding.h"
#include <Rush/scene/components/LightComponent.h>

namespace Rush {
namespace Bindings {

class LightComponent : public Component {
public:
    COMPONENT_BODY(LightComponent, ScriptingBackend::BindingsBaseDir + "components/LightComponent.cs");

    static float GetRange(MonoObject *instance);
    static void SetRange(MonoObject *instance, float range);

    static LightType GetType(MonoObject *instance);
    static void SetType(MonoObject *instance, LightType type);

    static bool GetManualAttenuation(MonoObject *instance);
    static void SetManualAttenuation(MonoObject *instance, bool enabled);

    static float GetConstant(MonoObject *instance);
    static void SetConstant(MonoObject *instance, float constant);

    static float GetLinear(MonoObject *instance);
    static void SetLinear(MonoObject *instance, float linear);

    static float GetQuadratic(MonoObject *instance);
    static void SetQuadratic(MonoObject *instance, float quadratic);

    static float GetCutoff(MonoObject *instance);
    static void SetCutoff(MonoObject *instance, float cutoff);

    static float GetOuterCutoff(MonoObject *instance);
    static void SetOuterCutoff(MonoObject *instance, float outerCutoff);

    static glm::vec3 GetAmbient(MonoObject *instance);
    static void SetAmbient(MonoObject *instance, glm::vec3 ambient);

    static glm::vec3 GetDiffuse(MonoObject *instance);
    static void SetDiffuse(MonoObject *instance, glm::vec3 diffuse);

    static glm::vec3 GetSpecular(MonoObject *instance);
    static void SetSpecular(MonoObject *instance, glm::vec3 specular);
};

} // namespace Bindings
} // namespace Rush

#endif // __LIGHTCOMPONENT_BINDING_H__
