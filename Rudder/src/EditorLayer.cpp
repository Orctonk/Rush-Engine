#include "EditorLayer.h"

#include <functional>
#include <imgui.h>

struct EntityName {
    std::string name;
};

struct Transform {
    float x = 0.f;
    float y = 0.f;
};

struct Velocity {
    float x = 0.f;
    float y = 0.f;
};

namespace MM {
template <>
void ComponentEditorWidget<Transform>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& t = reg.get<Transform>(e);
	ImGui::DragFloat("x", &t.x, 0.1f);
	ImGui::DragFloat("y", &t.y, 0.1f);
}

template <>
void ComponentEditorWidget<Velocity>(entt::registry& reg, entt::registry::entity_type e)
{
	auto& v = reg.get<Velocity>(e);
	ImGui::DragFloat("x", &v.x, 0.1f);
	ImGui::DragFloat("y", &v.y, 0.1f);
}
}



EditorLayer::EditorLayer() {}
EditorLayer::~EditorLayer() {}


void EditorLayer::OnAttach() {
    Rush::Application::GetInstance().GetRegistry().create();
    m_EntEditor.registerComponent<Transform>("Transform");
    m_EntEditor.registerComponent<Velocity>("Velocity");
}
void EditorLayer::OnDetach() {}
void EditorLayer::OnUpdate() {}
void EditorLayer::OnEvent(Rush::Event &e) {}
void EditorLayer::OnImguiRender() {
    bool show = true;
    ImGui::ShowDemoWindow(&show);

    entt::registry &reg = Rush::Application::GetInstance().GetRegistry();

    ImGui::Begin("Scene Editor");
    ImGui::Text("Scene graph");
    float width = ImGui::GetWindowWidth();
    ImGui::SameLine(width - 25.0f);
    if(ImGui::Button("+")) reg.create();
    std::function<void(const entt::entity)> rfunc = std::bind(&EditorLayer::RenderEntity,this,reg,std::placeholders::_2);
    
    reg.each(rfunc);
    
    ImGui::Separator();
    ImGui::BeginChild("Entity Editor");
    m_EntEditor.render(Rush::Application::GetInstance().GetRegistry(), m_SelectedEnt);
    ImGui::EndChild();
    ImGui::End();
}

void EditorLayer::RenderEntity(entt::registry &reg,const entt::entity e){
    const char *name;
    if(reg.has<EntityName>(e))
        name = reg.get<EntityName>(e).name.c_str();
    else
        name = std::to_string(entt::to_integral(e)).c_str();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if(e == m_SelectedEnt) flags |= ImGuiTreeNodeFlags_Selected;

    if(reg.has<entt::entity>(e)){
        if(ImGui::TreeNodeEx(name,flags)){
            RenderEntity(reg.get<entt::entity>(e));
            ImGui::TreePop();
        }
        
    } else {
        flags |= ImGuiTreeNodeFlags_Leaf;
        ImGui::TreeNodeEx(name,flags);
    }
    if(ImGui::IsItemClicked()){
        m_SelectedEnt = e;
    }
}