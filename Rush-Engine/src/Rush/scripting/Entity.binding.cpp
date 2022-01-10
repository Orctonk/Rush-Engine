#include "Entity.binding.h"

#include "Rush/scene/components/LightComponent.h"
#include "Rush/scene/components/TransformComponent.h"
#include "components/LightComponent.binding.h"
#include "components/TransformComponent.binding.h"

namespace Rush {
namespace Bindings {

void Entity::Init() {}

void Entity::BindMethods() {
    mono_add_internal_call("Rush.Entity::GetComponent_LightComponent", (const void *)GetLightComponent);
    mono_add_internal_call("Rush.Entity::GetComponent_TransformComponent", (const void *)GetTransformComponent);
}

Rush::Entity Entity::EntityFromMonoInstance(MonoObject *instance) {
    MonoClass *c = mono_class_from_name(ScriptingBackend::GetImage(), "Rush", "Entity");
    MonoClassField *rf = mono_class_get_field_from_name(c, "registry");
    MonoClassField *ef = mono_class_get_field_from_name(c, "entityHandle");
    entt::registry *reg;
    entt::entity ent;
    mono_field_get_value(instance, rf, &reg);
    mono_field_get_value(instance, ef, &ent);
    return Rush::Entity(reg, ent);
}

MonoObject *Entity::GetLightComponent(MonoObject *instance) {
    Rush::Entity ent = EntityFromMonoInstance(instance);
    if (!ent.HasComponent<::LightComponent>()) return nullptr;
    return LightComponent::CreateComponent(ent, &ent.GetComponent<::LightComponent>());
}

MonoObject *Entity::GetTransformComponent(MonoObject *instance) {
    Rush::Entity ent = EntityFromMonoInstance(instance);
    if (!ent.HasComponent<::TransformComponent>()) return nullptr;
    return TransformComponent::CreateComponent(ent, &ent.GetComponent<::TransformComponent>());
}

MonoObject *Entity::CreateEntity(Rush::Entity entity) {
    MonoClass *c = mono_class_from_name(ScriptingBackend::GetImage(), "Rush", "Entity");
    MonoClassField *rf = mono_class_get_field_from_name(c, "registry");
    MonoClassField *ef = mono_class_get_field_from_name(c, "entityHandle");
    MonoObject *e = mono_object_new(ScriptingBackend::GetAppDomain(), c);
    mono_runtime_object_init(e);
    entt::registry *reg = entity.GetParentRegistry();
    uint32_t ent = entity.GetID();
    mono_field_set_value(e, rf, &reg);
    mono_field_set_value(e, ef, &ent);
    return e;
}

} // namespace Bindings
} // namespace Rush
