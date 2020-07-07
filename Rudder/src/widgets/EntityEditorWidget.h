#ifndef __ENTITYEDITORWIDGET_H__
#define __ENTITYEDITORWIDGET_H__

#include <entt/entt.hpp>
#include <imgui.h>
#include <string>
#include <unordered_map>

typedef void(*ComponentFunc)(entt::registry &, entt::entity);

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
        d.create = [](entt::registry &reg, entt::entity e){reg.emplace<Comp>(e);};
        d.remove = [](entt::registry &reg, entt::entity e){
            reg.remove<Comp>(e);
            };
        m_CompMap.emplace(entt::type_info<Comp>::id(), d );
    }    

    void Render(entt::registry &reg, entt::entity e, bool *shown);
};

#endif // __ENTITYEDITORWIDGET_H__
