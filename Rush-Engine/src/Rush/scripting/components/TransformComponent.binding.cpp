#include "TransformComponent.binding.h"

#include "Rush/scene/components/TransformComponent.h"

namespace Rush {
namespace Bindings {

void TransformComponent::Init() {}

void TransformComponent::BindMethods() {
    mono_add_internal_call("Rush.TransformComponent::GetModelMatrix", (const void *)GetModelMatrix);
    mono_add_internal_call("Rush.TransformComponent::get_Translation", (const void *)GetTranslation);
    mono_add_internal_call("Rush.TransformComponent::get_Rotation", (const void *)GetRotation);
    mono_add_internal_call("Rush.TransformComponent::get_Scale", (const void *)GetScale);
    mono_add_internal_call("Rush.TransformComponent::set_Translation", (const void *)SetTranslation);
    mono_add_internal_call("Rush.TransformComponent::set_Rotation", (const void *)SetRotation);
    mono_add_internal_call("Rush.TransformComponent::set_Scale", (const void *)SetScale);
    mono_add_internal_call("Rush.TransformComponent::Translate", (const void *)Translate);
    mono_add_internal_call("Rush.TransformComponent::DoScale", (const void *)Scale);
    mono_add_internal_call("Rush.TransformComponent::Rotate", (const void *)Rotate);
}

glm::mat4 TransformComponent::GetModelMatrix(MonoObject *instance) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    return tc->GetModelMatrix();
}
glm::vec3 TransformComponent::GetTranslation(MonoObject *instance) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    return tc->GetTranslation();
}
glm::quat TransformComponent::GetRotation(MonoObject *instance) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    return tc->GetRotation();
}
glm::vec3 TransformComponent::GetScale(MonoObject *instance) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    return tc->GetScale();
}
void TransformComponent::SetTranslation(MonoObject *instance, const glm::vec3 &translation) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    tc->SetTranslation(translation);
}
void TransformComponent::SetRotation(MonoObject *instance, const glm::quat &rotation) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    tc->SetRotation(rotation);
}
void TransformComponent::SetScale(MonoObject *instance, const glm::vec3 &scale) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    tc->SetScale(scale);
}
void TransformComponent::Translate(MonoObject *instance, const glm::vec3 &translation) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    tc->Translate(translation);
}
void TransformComponent::Scale(MonoObject *instance, const glm::vec3 &scale) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    tc->Scale(scale);
}
void TransformComponent::Rotate(MonoObject *instance, const glm::quat &rotation) {
    ::TransformComponent *tc = static_cast<::TransformComponent *>(GetHandle(instance));
    tc->Rotate(rotation);
}

} // namespace Bindings
} // namespace Rush