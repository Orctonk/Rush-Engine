#include "TransformComponent.binding.h"

#include "Rush/scene/components/TransformComponent.h"

namespace Rush {
namespace Bindings {

void TransformComponent::Init() { }

void TransformComponent::BindMethods() {
    mono_add_internal_call("Rush.TransformComponent::GetModelMatrix", GetModelMatrix);
    mono_add_internal_call("Rush.TransformComponent::get_Translation", GetTranslation);
    mono_add_internal_call("Rush.TransformComponent::get_Rotation", GetRotation);
    mono_add_internal_call("Rush.TransformComponent::get_Scale", GetScale);
    mono_add_internal_call("Rush.TransformComponent::set_Translation", SetTranslation);
    mono_add_internal_call("Rush.TransformComponent::set_Rotation", SetRotation);
    mono_add_internal_call("Rush.TransformComponent::set_Scale", SetScale);
    mono_add_internal_call("Rush.TransformComponent::Translate", Translate);
    mono_add_internal_call("Rush.TransformComponent::DoScale", Scale);
    mono_add_internal_call("Rush.TransformComponent::Rotate", Rotate);
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