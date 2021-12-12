#include "Component.binding.h"
#include "../ScriptingBackend.h"
#include "../Entity.binding.h"
#include <iostream>

namespace Rush {
namespace Bindings {

void Component::Init() {}

void Component::BindMethods() {}

void *Component::GetHandle(MonoObject *instance) {
    MonoClass *c = mono_class_from_name(ScriptingBackend::GetImage(), "Rush", "Component");
    MonoClassField *f = mono_class_get_field_from_name(c, "native");
    void *component;
    mono_field_get_value(instance, f, &component);
    return component;
}

MonoObject *Component::CreateComponent(Rush::Entity parent, void *nativePtr, const char *typeName) {
    MonoClass *c = mono_class_from_name(ScriptingBackend::GetImage(), "Rush", typeName);
    MonoClassField *nativeHandleField = mono_class_get_field_from_name(c, "native");
    MonoProperty *parentProperty = mono_class_get_property_from_name(c, "parent");
    MonoObject *component = mono_object_new(ScriptingBackend::GetAppDomain(), c);
    MonoObject *parentObj = Entity::CreateEntity(parent);
    mono_runtime_object_init(component);
    mono_field_set_value(component, nativeHandleField, &nativePtr);
    void *args[1];
    args[0] = parentObj;
    mono_property_set_value(parentProperty, component, args, NULL);
    return component;
}

}
}