#include "EditorLayer.h"

#include "EditorComponents.h"
#include "FileBrowser.h"

#include "Rush/core/Input.h"
#include "Rush/events/KeyboardEvent.h"
#include "Rush/events/Keycodes.h"
#include "Rush/graphics/VertexArray.h"
#include "Rush/graphics/Renderer.h"
#include "Rush/scene/Mesh.h"
#include "Rush/scene/Transform.h"
#include "Rush/scene/Material.h"
#include "Rush/scene/Light.h"

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <glad/glad.h>

EditorLayer::EditorLayer() {}

EditorLayer::~EditorLayer() {}

void EditorLayer::OnAttach() {
    using namespace Rush;
    m_RenderViews.Init();

    auto &reg = Application::GetInstance().GetRegistry();

    auto globalLight = reg.create();
    reg.emplace<EntityName>(globalLight,"Global editor lighting");
    auto &l = reg.emplace<DirectionalLight>(globalLight);
    l.ambient = {0.2f,0.2f,0.2f};
    l.diffuse = {0.0f,0.0f,0.0f};
    l.specular = {0.0f,0.0f,0.0f};

    auto testLight = reg.create();
    reg.emplace<EntityName>(testLight, "Test point Light");
    auto &l2 = reg.emplace<PointLight>(testLight);
    l2.position = {1.0f,1.0f,1.0f};

    auto model = reg.create();
    m_Model = ResourceLoader::LoadModel("res/backpack/backpack.obj");
    auto &mater = reg.emplace<Material>(model,nullptr,nullptr,nullptr,8.0f);
    auto &m = reg.emplace<Mesh>(model,VertexArray::Create(),mater);

    reg.emplace<Transform>(model,glm::vec3(0.0f),glm::vec3(0.0f));
    reg.emplace<EntityName>(model, "Mesh Test");

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
    
}
void EditorLayer::OnDetach() {}
void EditorLayer::OnUpdate() {
    m_RenderViews.OnUpdate();
}

void EditorLayer::OnEvent(Rush::Event &e) {
    m_RenderViews.OnEvent(e);
    e.Dispatch<Rush::KeyboardPressEvent>(RUSH_BIND_FN(EditorLayer::KeyPressHandle));
}
void EditorLayer::OnImguiRender() {
    entt::registry &reg = Rush::Application::GetInstance().GetRegistry();

    ImGui::ShowDemoWindow();

    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("File")){
        if(ImGui::MenuItem("New")); // TODO: Implement New button
        if(ImGui::MenuItem("Save")); // TODO: Implement Save button
        if(ImGui::MenuItem("Open")); // TODO: Implement Open button
        if(ImGui::BeginMenu("Open recent")){
            if(ImGui::MenuItem("recent_file_1.scene")); // TODO: Implement Open recent button
            if(ImGui::MenuItem("recent_file_2.scene"));
            if(ImGui::MenuItem("recent_file_3.scene"));
            if(ImGui::MenuItem("recent_file_4.scene"));
            ImGui::EndMenu();
        } 
        ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("Edit")){
        if(ImGui::MenuItem("Undo")); // TODO: Implement Undo button
        if(ImGui::MenuItem("Redo")); // TODO: Implement Redo button
        if(ImGui::MenuItem("Copy")); // TODO: Implement Copy button
        if(ImGui::MenuItem("Cut")); // TODO: Implement Cut button
        if(ImGui::MenuItem("Paste")); // TODO: Implement Paste button
        ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("View")){
        if(ImGui::BeginMenu("Render views")){
            ImGui::MenuItem("Render preview", "", &m_RenderViews.enabledViews[RENDERVIEW_RENDER]);
            ImGui::MenuItem("Normals", "", &m_RenderViews.enabledViews[RENDERVIEW_NORMALS]);
            ImGui::MenuItem("Albedo", "", &m_RenderViews.enabledViews[RENDERVIEW_ALBEDO]);
            ImGui::MenuItem("Specular", "", &m_RenderViews.enabledViews[RENDERVIEW_SPECULAR]);
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus 
        | ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f));
    ImGui::Begin("Main window",NULL,flags);
    ImGui::DockSpace(ImGui::GetID("Main window dockspace"),ImVec2(0.0f,0.0f),ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
    ImGui::PopStyleVar();

    m_RenderViews.OnImguiRender();
    
    ImGui::Begin("Scene Editor");
    ImGui::Text("Scene graph");
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
    
    ImGui::Separator();
    m_EE.Render(reg,m_SelectedEnt);
    ImGui::End();
}

void EditorLayer::RenderEntity(const entt::entity e){
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

bool EditorLayer::KeyPressHandle(Rush::KeyboardPressEvent &e){
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
