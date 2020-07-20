#include "SceneGraphView.h"
#include "EditorComponents.h"
#include "widgets/FileBrowser.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

SceneGraphView::SceneGraphView(){ 
    using namespace Rush;
    m_SelectedEnt = Scene::nullEnt;
    m_EE.Register<Transform>("Transform",[](Rush::Scene &scene, Rush::Scene::EntityType e){
        auto& t = scene.Get<Transform>(e);
        ImGui::DragFloat3("Position", &t.translation.x, 0.01f);
        ImGui::DragFloat3("Rotation", &t.rotation.x, 1.0f);
        ImGui::DragFloat3("Scale", &t.scale.x, 0.01f);
    });
    m_EE.Register<MeshInstance>("MeshInstance",[](Rush::Scene &scene, Rush::Scene::EntityType e){
        for(auto &mesh : scene.Get<MeshInstance>(e).mesh->submeshes){
            if(ImGui::TreeNode(mesh.meshName.c_str())){
                ImGui::Text("Mesh data...");
                ImGui::Separator();
                SceneGraphView::RenderMaterial(*mesh.material);
                ImGui::TreePop();
            }
        }
    });
    m_EE.Register<DirectionalLight>("Directional light",[](Rush::Scene &scene, Rush::Scene::EntityType e){
        auto& l = scene.Get<DirectionalLight>(e);
        ImGui::DragFloat3("Direction", &l.direction.x, 0.1f);
        ImGui::ColorEdit3("Ambient", &l.ambient.r);
        ImGui::ColorEdit3("Diffuse", &l.diffuse.r);
        ImGui::ColorEdit3("Specular", &l.specular.r);
    });
    m_EE.Register<PointLight>("Point light",[](Rush::Scene &scene, Rush::Scene::EntityType e){
        auto& l = scene.Get<PointLight>(e);
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

void SceneGraphView::OnImguiRender(Rush::Scene &scene){
    using namespace Rush; 
    m_SceneRenderAccess = &scene;
    if(!enabled)
        return;
    
    ImGui::Begin("Scene graph",&enabled);
    float width = ImGui::GetWindowWidth();
    ImGui::SameLine(width - 50.0f);
    if(ImGui::Button("+")) scene.NewEntity();
    ImGui::SameLine();
    if(ImGui::Button("-") && m_SelectedEnt != scene.nullEnt) {
        scene.DeleteEntity(m_SelectedEnt);
        m_SelectedEnt = scene.nullEnt;
    }

    scene.ForEach([=](const entt::entity e){
        RenderEntity(e);
    });

    ImGui::End();
    
    if(m_SelectedEnt != scene.nullEnt && m_EEVisible)
        m_EE.Render(scene,m_SelectedEnt,&m_EEVisible);
}

void SceneGraphView::RenderEntity(const Rush::Scene::EntityType e){
    const char *name = "";
    Rush::Scene &scene = *m_SceneRenderAccess;
    if(scene.Has<EntityName>(e))
        name = scene.Get<EntityName>(e).name.c_str();
    if (strlen(name) == 0)
        name = std::to_string(entt::to_integral(e)).c_str();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if(e == m_SelectedEnt) {
        flags |= ImGuiTreeNodeFlags_Selected;
        if(m_Renaming){
            name = "##renaming";
        }
    }
    if(false){ // Nested entities
        // if(ImGui::TreeNodeEx(name,flags)){
        //     RenderEntity(reg.get<entt::entity>(e));
        //     ImGui::TreePop();
        // }
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
                scene.NewEntity();
            }
            if(ImGui::MenuItem("Delete")){
                scene.DeleteEntity(e);
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
        if(!scene.Has<EntityName>(e))
            scene.Add<EntityName>(e);
    
        EntityName &name = scene.Get<EntityName>(e);
        ImGui::SameLine();
        ImGui::InputText("##rename_field",&name.name);
        ImGui::SetKeyboardFocusHere();

        if(ImGui::IsMouseClicked(0) && !ImGui::IsItemClicked()){
            m_Renaming = false;
        }
    }
}

void SceneGraphView::RenderMaterial(Rush::Material &mat) {
    using namespace Rush;
        ImGui::Text("Diffuse:  ");
        ImGui::SameLine();
        if(mat.diffuseTexture == nullptr)
            ImGui::Button("None");
        else
            ImGui::Button( mat.diffuseTexture->GetDebugPath().c_str());

        static FileBrowser diffDialog;
        if(ImGui::IsItemClicked()){
            diffDialog.SetTitle("Select Diffuse map...");
            diffDialog.Open();
        }
        diffDialog.Render();
        if(diffDialog.Finished()){
            mat.diffuseTexture = AssetManager::GetTexture(diffDialog.GetSelectedFile().c_str());
        }

        ImGui::Text("Specular: ");
        ImGui::SameLine();
        if(mat.specularTexture == nullptr)
            ImGui::Button("None");
        else
            ImGui::Button(mat.specularTexture->GetDebugPath().c_str());

        static FileBrowser specDialog;
        if(ImGui::IsItemClicked()){
            specDialog.SetTitle("Select Specular map...");
            specDialog.Open();
        }
        specDialog.Render();
        if(specDialog.Finished()){
            mat.specularTexture = AssetManager::GetTexture(specDialog.GetSelectedFile().c_str());
        }

        ImGui::Text("Normal: ");
        ImGui::SameLine();
        if(mat.normalTexture == nullptr)
            ImGui::Button("None");
        else
            ImGui::Button(mat.normalTexture->GetDebugPath().c_str());

        static FileBrowser normDialog;
        if(ImGui::IsItemClicked()){
            normDialog.SetTitle("Select Normal map...");
            normDialog.Open();
        }
        normDialog.Render();
        if(normDialog.Finished()){
            mat.normalTexture = AssetManager::GetTexture(normDialog.GetSelectedFile().c_str());
        }

        ImGui::DragFloat("Shininess",&mat.shininess,0.1f,0.0f);
}

bool SceneGraphView::KeyPressHandle(Rush::KeyboardPressEvent &e){
    // if(e.keycode == RUSH_KEY_DELETE && m_SelectedEnt != entt::null){
    //     reg.destroy(m_SelectedEnt);
    //     m_SelectedEnt = entt::null;
    //     return true;
    // }
    // else if(e.keycode == RUSH_KEY_F2 && m_SelectedEnt != entt::null){
    //     m_Renaming = true;
    //     return true;
    // }
    // else if(e.keycode == RUSH_KEY_ENTER && m_Renaming){
    //     m_Renaming = false;
    //     return true;
    // }
    return false;
}