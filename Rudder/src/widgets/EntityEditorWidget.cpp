#include "EntityEditorWidget.h"
#include "views/EditorComponents.h"

EntityEditor::EntityEditor(){

}

EntityEditor::~EntityEditor(){
    
}

void EntityEditor::Render(Rush::Scene &scene, Rush::Scene::EntityType e, bool *shown){
    ImGui::Begin("Entity editor",shown);
    if(e == Rush::Scene::nullEnt){
        ImGui::End();
        return;
    }
    const char * name = "";
    if(scene.Has<EntityName>(e))
        name = scene.Get<EntityName>(e).name.c_str();
    if(strlen(name) == 0)
        ImGui::Text("Entity: %d", entt::to_integral(e));
    else
        ImGui::Text(name);

    std::vector<entt::id_type> unused;
    for(auto &[id, data] : m_CompMap){
        ImGui::PushID("Widget");
        entt::id_type c[] = {id};
        if(scene.GetRegistry()->runtime_view(std::begin(c),std::end(c)).contains(e)){
            if(ImGui::Button("-")) {
                data.remove(scene,e);
                ImGui::PopID();
                continue;
            }
            ImGui::SameLine();
            if(ImGui::CollapsingHeader(data.name.c_str())) data.drawWidget(scene,e);
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
                    m_CompMap.at(id).create(scene,e);
                }
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}