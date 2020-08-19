#include "GlobalEntitySelection.h"
#include "SceneGraphView.h"
#include "EditorComponents.h"
#include "widgets/FileBrowser.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

SceneGraphView::SceneGraphView(){ 
    using namespace Rush;
    m_EE.Register<TransformComponent>("Transform",[](Rush::Entity &e){
        auto& t = e.GetComponent<TransformComponent>();
        glm::quat qrot = t.GetRotation();
        glm::vec3 trans = t.GetTranslation();
        glm::vec3 rot;
        rot.x = glm::degrees(glm::pitch(qrot));
        rot.y = glm::degrees(glm::yaw(qrot));
        rot.z = glm::degrees(glm::roll(qrot));
        glm::vec3 scale = t.GetScale();
        if(ImGui::DragFloat3("Position", &trans.x, 0.01f))
            t.SetTranslation(trans);
        if(ImGui::DragFloat3("Rotation", &rot.x, 1.0f))
            t.SetRotation(glm::radians(rot));
        if(ImGui::DragFloat3("Scale", &scale.x, 0.01f))
            t.SetScale(scale);
    });
    m_EE.Register<CameraComponent>("Camera",[](Rush::Entity &e){
        auto& c = e.GetComponent<CameraComponent>();
        bool modified = false;
        float near = c.camera.GetNear();
        float far = c.camera.GetFar();

        ImGui::Checkbox("Main", &c.main);
        ImGui::ColorEdit4("Clear color",&c.clearColor.r);

        int selProj = c.camera.IsPerspective() ? 0 : 1;
        if(ImGui::Combo("Projection", &selProj,"Perspective\0Orthographic\0\0")){
            if(selProj == c.camera.IsPerspective()){
                if(selProj == 0)    c.camera.SetPerspective(1024.0f/720.0f,45.0f);
                else                c.camera.SetOrthographic(0.0f,1.0f,1.0f,0.0f);
            }
        }

        if(c.camera.IsPerspective()){
            float fov = c.camera.GetFOV();
            float aspect = c.camera.GetAspect();
            if(ImGui::SliderFloat("FOV", &fov,10.0f,120.0f,"%.0f")) modified = true;
            if(ImGui::DragFloat("Aspect", &aspect,0.1f)) modified = true;
            if(ImGui::DragFloat("Near", &near,0.1f)) modified = true;
            if(ImGui::DragFloat("Far", &far,0.1f)) modified = true;
            if(modified) c.camera.SetPerspective(aspect,fov,near,far);
        } else {
            float left = c.camera.GetLeft();
            float right = c.camera.GetRight();
            float top = c.camera.GetTop();
            float bottom = c.camera.GetBottom();
            if(ImGui::DragFloat("Left", &left,0.1f)) modified = true;
            if(ImGui::DragFloat("Right", &right,0.1f)) modified = true;
            if(ImGui::DragFloat("Top", &top,0.1f)) modified = true;
            if(ImGui::DragFloat("Bottom", &bottom,0.1f)) modified = true;
            if(ImGui::DragFloat("Near", &near,0.1f)) modified = true;
            if(ImGui::DragFloat("Far", &far,0.1f)) modified = true;
            if(modified) c.camera.SetOrthographic(left,right,top,bottom,near,far);
        }
    });
    m_EE.Register<MeshInstance>("MeshInstance",[](Rush::Entity &e){
        for(auto &mesh : e.GetComponent<MeshInstance>().mesh->submeshes){
            if(ImGui::TreeNode(mesh.meshName.c_str())){
                ImGui::Text("Mesh data...");
                ImGui::Separator();
                SceneGraphView::RenderMaterial(*mesh.material.parent);
                ImGui::TreePop();
            }
        }
    });
    m_EE.Register<LightComponent>("Light",[](Rush::Entity &e){
        auto& l = e.GetComponent<LightComponent>();
        int curType = (int)l.type;
        if(ImGui::Combo("Light type",&curType,"Directional\0Point\0Spotlight\0\0")){
            l.type = (LightType)curType;
        }
        ImGui::ColorEdit3("Ambient", &l.ambient.r);
        ImGui::ColorEdit3("Diffuse", &l.diffuse.r);
        ImGui::ColorEdit3("Specular", &l.specular.r);
        if(l.type != LightType::DIRECTIONAL){
            ImGui::Checkbox("Manual Attenuation",&l.manualAtten);
            if(ImGui::DragFloat("Range",&l.range,1.0f,0.0000001f,10000.0f) && !l.manualAtten){
                l.constant = 1.0f;
                l.linear = 4.5f / l.range;
                l.quadratic = 75.0f / (l.range * l.range);
            } 
            if(l.manualAtten) {
                ImGui::Text("Attenuation");
                ImGui::DragFloat3("Coef.",&l.constant,0.01f,0.0f);
            }
        }
        if(l.type == LightType::SPOTLIGHT){
            ImGui::Text("Spotlight cutoff");
            ImGui::DragFloat2("Cutoff",&l.cutOff);
        }
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
    if(!enabled)
        return;
    
    ImGui::Begin("Scene graph",&enabled);
    float width = ImGui::GetWindowWidth();
    ImGui::SameLine(width - 50.0f);
    if(ImGui::Button("+")) scene.NewEntity();
    ImGui::SameLine();
    if(ImGui::Button("-") && GlobalEntitySelection::GetSelection()) {
        scene.DeleteEntity(GlobalEntitySelection::GetSelection());
        GlobalEntitySelection::ClearSelection();
    }

    scene.GetRegistry()->each([&](entt::entity e){
        RenderEntity({scene.GetRegistry(), e});
    });

    ImGui::End();
    
    if(GlobalEntitySelection::GetSelection() && m_EEVisible)
        m_EE.Render(GlobalEntitySelection::GetSelection(),&m_EEVisible);
}

void SceneGraphView::RenderEntity(Rush::Entity e){
    const char *name = "";
    if(e.HasComponent<EntityName>())
        name = e.GetComponent<EntityName>().name.c_str();
    if (strlen(name) == 0)
        name = std::to_string(e.GetID()).c_str();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if(GlobalEntitySelection::IsSelection(e)) {
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
        if(!(GlobalEntitySelection::IsSelection(e)))
            m_Renaming = false;
        m_EEVisible = true;
        GlobalEntitySelection::SetSelection(e);
        if(ImGui::IsMouseDoubleClicked(0)){
            m_Renaming = true;
        }
    }
    if(m_Renaming && GlobalEntitySelection::IsSelection(e)){
        if(!e.HasComponent<EntityName>())
            e.AddComponent<EntityName>();
    
        EntityName &name = e.GetComponent<EntityName>();
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