#include "EntityEditorWidget.h"
#include "views/EditorComponents.h"

EntityEditor::EntityEditor(){

}

EntityEditor::~EntityEditor(){
    
}

void EntityEditor::Render(entt::registry &reg, entt::entity e, bool *shown){
    ImGui::Begin("Entity editor",shown);
    if(e == entt::null){
        ImGui::End();
        return;
    }
    const char * name = "";
    if(reg.has<EntityName>(e))
        name = reg.get<EntityName>(e).name.c_str();
    if(strlen(name) == 0)
        ImGui::Text("Entity: %d", entt::to_integral(e));
    else
        ImGui::Text(name);

    std::vector<entt::id_type> unused;
    for(auto &[id, data] : m_CompMap){
        ImGui::PushID("Widget");
        entt::id_type c[] = {id};
        if(reg.runtime_view(std::begin(c),std::end(c)).contains(e)){
            if(ImGui::Button("-")) {
                data.remove(reg,e);
                ImGui::PopID();
                continue;
            }
            ImGui::SameLine();
            if(ImGui::CollapsingHeader(data.name.c_str())) data.drawWidget(reg,e);
        } else {
            unused.push_back(id);
        }
        ImGui::PopID();
    }
    if(!unused.empty()){
        if(ImGui::Button("Add Component")){
            ImGui::OpenPopup("Comp popup");
        }
        if(ImGui::BeginPopup("Comp popup")){
            for(auto id : unused){
                if(ImGui::Button(m_CompMap.at(id).name.c_str())){
                    m_CompMap.at(id).create(reg,e);
                }
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}