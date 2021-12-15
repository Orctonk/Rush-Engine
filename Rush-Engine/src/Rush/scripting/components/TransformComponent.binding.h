#ifndef __TRANSFORMCOMPONENT_BINDING_H__
#define __TRANSFORMCOMPONENT_BINDING_H__

#include "Component.binding.h"

#include <glm/glm.hpp>

namespace Rush {
namespace Bindings {

class TransformComponent : public Component {
public:
    COMPONENT_BODY(TransformComponent, ScriptingBackend::BindingsBaseDir + "components/TransformComponent.cs");

    static glm::mat4 GetModelMatrix(MonoObject *instance);
    static glm::vec3 GetTranslation(MonoObject *instance);
    static glm::quat GetRotation(MonoObject *instance);
    static glm::vec3 GetScale(MonoObject *instance);
    static void SetTranslation(MonoObject *instance, const glm::vec3 &translation);
    static void SetRotation(MonoObject *instance, const glm::quat &rotation);
    static void SetScale(MonoObject *instance, const glm::vec3 &scale);
    static void Translate(MonoObject *instance, const glm::vec3 &translation);
    static void Scale(MonoObject *instance, const glm::vec3 &scale);
    static void Rotate(MonoObject *instance, const glm::quat &rotation);
};

} // namespace Bindings
} // namespace Rush

#endif // __TRANSFORMCOMPONENT_BINDING_H__
