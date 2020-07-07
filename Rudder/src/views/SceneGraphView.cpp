#include "SceneGraphView.h"
#include "EditorComponents.h"
#include "widgets/FileBrowser.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

SceneGraphView::SceneGraphView(){ 
    using namespace Rush;
    m_SelectedEnt = entt::null;
    m_EE.Register<Transform>("Transform",[](entt::registry &reg,entt::entity e){
        auto& t = reg.get<Transform>(e);
        ImGui::DragFloat3("Position", &t.translation.x, 0.01f);
        ImGui::DragFloat3("Rotation", &t.rotation.x, 1.0f);
        ImGui::DragFloat3("Scale", &t.scale.x, 0.01f);
    });
    m_EE.Register<Material>("Material",[](entt::registry &reg, entt::entity e){
        auto& m = reg.get<Material>(e);
        ImGui::Text("Diffuse:  ");
        ImGui::SameLine();
        if(m.diffuseTexture == nullptr)
            ImGui::Button("None");
        else
            ImGui::Button( m.diffuseTexture->GetDebugPath().c_str());

        static FileBrowser diffDialog;
        if(ImGui::IsItemClicked()){
            diffDialog.SetTitle("Select Diffuse map...");
            diffDialog.Open();
        }
        diffDialog.Render();
        if(diffDialog.Finished()){
            m.diffuseTexture = ResourceLoader::LoadTexture(diffDialog.GetSelectedFile().c_str());
        }

        ImGui::Text("Specular: ");
        ImGui::SameLine();
        if(m.specularTexture == nullptr)
            ImGui::Button("None");
        else
            ImGui::Button(m.specularTexture->GetDebugPath().c_str());

        static FileBrowser specDialog;
        if(ImGui::IsItemClicked()){
            specDialog.SetTitle("Select Specular map...");
            specDialog.Open();
        }
        specDialog.Render();
        if(specDialog.Finished()){
            m.specularTexture = ResourceLoader::LoadTexture(specDialog.GetSelectedFile().c_str());
        }

        ImGui::Text("Normal: ");
        ImGui::SameLine();
        if(m.normalTexture == nullptr)
            ImGui::Button("None");
        else
            ImGui::Button(m.normalTexture->GetDebugPath().c_str());

        static FileBrowser normDialog;
        if(ImGui::IsItemClicked()){
            normDialog.SetTitle("Select Normal map...");
            normDialog.Open();
        }
        normDialog.Render();
        if(normDialog.Finished()){
            m.normalTexture = ResourceLoader::LoadTexture(normDialog.GetSelectedFile().c_str());
        }

        ImGui::DragFloat("Shininess",&m.shininess,0.1f,0.0f);
    });
    m_EE.Register<DirectionalLight>("Directional light",[](entt::registry &reg,entt::entity e){
        auto& l = reg.get<DirectionalLight>(e);
        ImGui::DragFloat3("Direction", &l.direction.x, 0.1f);
        ImGui::ColorEdit3("Ambient", &l.ambient.r);
        ImGui::ColorEdit3("Diffuse", &l.diffuse.r);
        ImGui::ColorEdit3("Specular", &l.specular.r);
    });
    m_EE.Register<PointLight>("Point light",[](entt::registry &reg,entt::entity e){
        auto& l = reg.get<PointLight>(e);
        ImGui::DragFloat3("Position", &l.position.x, 0.1f);
        ImGui::ColorEdit3("Ambient", &l.ambient.r);
        ImGui::ColorEdit3("Diffuse", &l.diffuse.r);
        ImGui::ColorEdit3("Specular", &l.specular.r);
        ImGui::DragFloat3("Coef.", &l.constant,0.01f,0.0f);
    });
    m_Renaming = false;   
    enabled = true; // TODO: Make window open status persistent over application close
}

SceneGraphView::~SceneGraphView() { }

void SceneGraphView::OnEvent(Rush::Event &e){
    e.Dispatch<Rush::KeyboardPressEvent>(RUSH_BIND_FN(SceneGraphView::KeyPressHandle));
}

void SceneGraphView::OnImguiRender(){
    using namespace Rush; 
    if(!enabled)
        return;
    
    auto &reg = Application::GetInstance().GetRegistry();
    ImGui::Begin("Scene graph",&enabled);
    float width = ImGui::GetWindowWidth();
    ImGui::SameLine(width - 50.0f);
    if(ImGui::Button("+")) reg.create();
    ImGui::SameLine();
    if(ImGui::Button("-") && m_SelectedEnt != entt::null) {
        reg.remove_all(m_SelectedEnt);
        reg.destroy(m_SelectedEnt);
        m_SelectedEnt = entt::null;
    }

    reg.each([=](const entt::entity e){
        RenderEntity(e);
    });

    ImGui::End();
    
    if(m_SelectedEnt != entt::null && m_EEVisible)
        m_EE.Render(reg,m_SelectedEnt,&m_EEVisible);
}

void SceneGraphView::RenderEntity(const entt::entity e){
    entt::registry &reg = Rush::Application::GetInstance().GetRegistry();
    const char *name = "";
    if(reg.has<EntityName>(e))
        name = reg.get<EntityName>(e).name.c_str();
    if (strlen(name) == 0)
        name = std::to_string(entt::to_integral(e)).c_str();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if(e == m_SelectedEnt) {
        flags |= ImGuiTreeNodeFlags_Selected;
        if(m_Renaming){
            name = "##renaming";
        }
    }
    if(reg.has<entt::entity>(e)){
        if(ImGui::TreeNodeEx(name,flags)){
            RenderEntity(reg.get<entt::entity>(e));
            ImGui::TreePop();
        }
    } else {
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx(name,flags);
    }
    if(ImGui::IsItemClicked() || ImGui::IsItemClicked(1)){
        if(e != m_SelectedEnt)
            m_Renaming = false;
        m_EEVisible = true;
        m_SelectedEnt = e;
        if(ImGui::IsMouseDoubleClicked(0)){
            m_Renaming = true;
        }
    }
    
    if(e == m_SelectedEnt){
        if(ImGui::BeginPopupContextItem()){
            if(ImGui::MenuItem("New Entity")){
                reg.create();
            }
            if(ImGui::MenuItem("Delete")){
                reg.destroy(e);
                m_SelectedEnt = entt::null;
            }
            if(ImGui::MenuItem("Rename")){
                m_Renaming = true;
                m_SelectedEnt = e;
            }
            ImGui::EndPopup();
        }
    } 
    if(m_Renaming && e == m_SelectedEnt){
        if(!reg.has<EntityName>(e))
            reg.emplace<EntityName>(e);
    
        EntityName &name = reg.get<EntityName>(e);
        ImGui::SameLine();
        ImGui::InputText("##rename_field",&name.name);
        ImGui::SetKeyboardFocusHere();

        if(ImGui::IsMouseClicked(0) && !ImGui::IsItemClicked()){
            m_Renaming = false;
        }
    }
}

bool SceneGraphView::KeyPressHandle(Rush::KeyboardPressEvent &e){
    entt::registry &reg = Rush::Application::GetInstance().GetRegistry();
    if(e.keycode == RUSH_KEY_DELETE && m_SelectedEnt != entt::null){
        reg.destroy(m_SelectedEnt);
        m_SelectedEnt = entt::null;
        return true;
    }
    else if(e.keycode == RUSH_KEY_F2 && m_SelectedEnt != entt::null){
        m_Renaming = true;
        return true;
    }
    else if(e.keycode == RUSH_KEY_ENTER && m_Renaming){
        m_Renaming = false;
        return true;
    }
    return false;
}