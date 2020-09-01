#include "EntityEditorWidget.h"

EntityEditor::EntityEditor(){

}

EntityEditor::~EntityEditor(){
    
}

void EntityEditor::Render(Rush::Entity e, bool *shown){
    ImGui::Begin("Entity editor",shown);
    if(!e){
        ImGui::End();
        return;
    }
    std::string name;
    if(e.HasComponent<TagComponent>())
        name = e.GetComponent<TagComponent>().tag;
    if(name.empty())
        ImGui::Text("Entity: %d", e.GetID());
    else if(name == "New entity")
        ImGui::Text("%s (%d)", name.c_str(), e.GetID());
    else
        ImGui::Text(name.c_str());

    std::vector<entt::id_type> unused;
    for(auto &[id, data] : m_CompMap){
        ImGui::PushID("Widget");
        entt::id_type c[] = {id};
        if(e.GetParentRegistry()->runtime_view(std::begin(c),std::end(c)).contains(e)){
            if(ImGui::Button("-")) {
                data.remove(e);
                ImGui::PopID();
                continue;
            }
            ImGui::SameLine();
            if(ImGui::CollapsingHeader(data.name.c_str())) data.drawWidget(e);
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
                    m_CompMap.at(id).create(e);
                }
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}