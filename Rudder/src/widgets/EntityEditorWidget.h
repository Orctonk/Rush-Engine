#ifndef __ENTITYEDITORWIDGET_H__
#define __ENTITYEDITORWIDGET_H__

#include <Rush.h>
#include <entt/entt.hpp>
#include <imgui.h>
#include <string>
#include <unordered_map>

typedef void(*ComponentFunc)(Rush::Scene &, Rush::Scene::EntityType);

struct ComponentData {
    std::string name;
    ComponentFunc drawWidget;
    ComponentFunc create;
    ComponentFunc remove;
};

class EntityEditor {
    std::unordered_map<entt::id_type,ComponentData> m_CompMap;

public:
    EntityEditor();
    ~EntityEditor();

    template<typename Comp>
    void Register(std::string name, ComponentFunc draw){
        ComponentData d;
        d.name = name;
        d.drawWidget = draw;
        d.create = [](Rush::Scene &scene, Rush::Scene::EntityType e){scene.Add<Comp>(e);};
        d.remove = [](Rush::Scene &scene, Rush::Scene::EntityType e){
            scene.Remove<Comp>(e);
            };
        m_CompMap.emplace(entt::type_info<Comp>::id(), d );
    }    

    void Render(Rush::Scene &scene, Rush::Scene::EntityType e, bool *shown);
};

#endif // __ENTITYEDITORWIDGET_H__
