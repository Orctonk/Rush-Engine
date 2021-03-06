#ifndef __ENTITYEDITORWIDGET_H__
#define __ENTITYEDITORWIDGET_H__

#include <Rush.h>
#include <entt/entt.hpp>
#include <string>
#include <unordered_map>

typedef void(*ComponentFunc)(Rush::Entity &);

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
        d.create = [](Rush::Entity &e){e.AddComponent<Comp>();};
        d.remove = [](Rush::Entity &e){e.RemoveComponent<Comp>();};
        m_CompMap.emplace(entt::type_hash<Comp>::value(), d);
    }    

    void Render(Rush::Entity e, bool *shown);
};

#endif // __ENTITYEDITORWIDGET_H__
