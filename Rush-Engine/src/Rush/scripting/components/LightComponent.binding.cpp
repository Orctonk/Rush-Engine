#include "LightComponent.binding.h"

namespace Rush {
namespace Bindings {

void LightComponent::Init() {}

void LightComponent::BindMethods() {
    mono_add_internal_call("Rush.LightComponent::get_Range", (const void *)GetRange);
    mono_add_internal_call("Rush.LightComponent::set_Range", (const void *)SetRange);
    mono_add_internal_call("Rush.LightComponent::get_Type", (const void *)GetType);
    mono_add_internal_call("Rush.LightComponent::set_Type", (const void *)SetType);
    mono_add_internal_call("Rush.LightComponent::get_ManualAttenuation", (const void *)GetManualAttenuation);
    mono_add_internal_call("Rush.LightComponent::set_ManualAttenuation", (const void *)SetManualAttenuation);
    mono_add_internal_call("Rush.LightComponent::get_Constant", (const void *)GetConstant);
    mono_add_internal_call("Rush.LightComponent::set_Constant", (const void *)SetConstant);
    mono_add_internal_call("Rush.LightComponent::get_Linear", (const void *)GetLinear);
    mono_add_internal_call("Rush.LightComponent::set_Linear", (const void *)SetLinear);
    mono_add_internal_call("Rush.LightComponent::get_Quadratic", (const void *)GetQuadratic);
    mono_add_internal_call("Rush.LightComponent::set_Quadratic", (const void *)SetQuadratic);
    mono_add_internal_call("Rush.LightComponent::get_Cutoff", (const void *)GetCutoff);
    mono_add_internal_call("Rush.LightComponent::set_Cutoff", (const void *)SetCutoff);
    mono_add_internal_call("Rush.LightComponent::get_OuterCutoff", (const void *)GetOuterCutoff);
    mono_add_internal_call("Rush.LightComponent::set_OuterCutoff", (const void *)SetOuterCutoff);
    mono_add_internal_call("Rush.LightComponent::get_Ambient", (const void *)GetAmbient);
    mono_add_internal_call("Rush.LightComponent::set_Ambient", (const void *)SetAmbient);
    mono_add_internal_call("Rush.LightComponent::get_Diffuse", (const void *)GetDiffuse);
    mono_add_internal_call("Rush.LightComponent::set_Diffuse", (const void *)SetDiffuse);
    mono_add_internal_call("Rush.LightComponent::get_Specular", (const void *)GetSpecular);
    mono_add_internal_call("Rush.LightComponent::set_Specular", (const void *)SetSpecular);
};

float LightComponent::GetRange(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->range;
}

void LightComponent::SetRange(MonoObject *instance, float range) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->range = range;
    if (component->manualAttenuation) {
        component->constant = 1.0f;
        component->linear = 4.5f / component->range;
        component->quadratic = 75.0f / (component->range * component->range);
    }
}

LightType LightComponent::GetType(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->type;
}
void LightComponent::SetType(MonoObject *instance, LightType type) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->type = type;
}
bool LightComponent::GetManualAttenuation(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->manualAttenuation;
}
void LightComponent::SetManualAttenuation(MonoObject *instance, bool enabled) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->manualAttenuation = enabled;
    if (component->manualAttenuation) {
        component->constant = 1.0f;
        component->linear = 4.5f / component->range;
        component->quadratic = 75.0f / (component->range * component->range);
    }
}
float LightComponent::GetConstant(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->constant;
}
void LightComponent::SetConstant(MonoObject *instance, float constant) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->constant = constant;
}
float LightComponent::GetLinear(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->linear;
}
void LightComponent::SetLinear(MonoObject *instance, float linear) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->linear = linear;
}
float LightComponent::GetQuadratic(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->quadratic;
}
void LightComponent::SetQuadratic(MonoObject *instance, float quadratic) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->quadratic = quadratic;
}
float LightComponent::GetCutoff(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->cutoff;
}
void LightComponent::SetCutoff(MonoObject *instance, float cutoff) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->cutoff = cutoff;
}
float LightComponent::GetOuterCutoff(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->outerCutoff;
}
void LightComponent::SetOuterCutoff(MonoObject *instance, float outerCutoff) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->outerCutoff = outerCutoff;
}
glm::vec3 LightComponent::GetAmbient(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->ambient;
}
void LightComponent::SetAmbient(MonoObject *instance, glm::vec3 ambient) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->ambient = ambient;
}
glm::vec3 LightComponent::GetDiffuse(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->diffuse;
}
void LightComponent::SetDiffuse(MonoObject *instance, glm::vec3 diffuse) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->diffuse = diffuse;
}
glm::vec3 LightComponent::GetSpecular(MonoObject *instance) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    return component->specular;
}
void LightComponent::SetSpecular(MonoObject *instance, glm::vec3 specular) {
    ::LightComponent *component = reinterpret_cast<::LightComponent *>(GetHandle(instance));
    component->specular = specular;
}

} // namespace Bindings
} // namespace Rush
