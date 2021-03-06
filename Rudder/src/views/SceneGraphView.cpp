#include "Rudderpch.h"
#include "SceneGraphView.h"

#include "GlobalEntitySelection.h"
#include "widgets/FileBrowser.h"

#include <misc/cpp/imgui_stdlib.h>

#define PROPERTY_LABEL(x) ImGui::Text(x); ImGui::SameLine(100.0f); ImGui::SetNextItemWidth(150.0f)

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
        PROPERTY_LABEL("Position:");
        if(ImGui::DragFloat3("##Position", &trans.x, 0.01f,0.0f,0.0f,"%.2f"))
            t.SetTranslation(trans);
        PROPERTY_LABEL("Rotation:");
        if(ImGui::DragFloat3("##Rotation", &rot.x, 1.0f, 0.0f,0.0f,"%.1f"))
            t.SetRotation(glm::radians(rot));
        PROPERTY_LABEL("Scale:");
        if(ImGui::DragFloat3("##Scale", &scale.x, 0.01f,0.0f,0.0f,"%.2f"))
            t.SetScale(scale);
    });
    m_EE.Register<CameraComponent>("Camera",[](Rush::Entity &e){
        auto& c = e.GetComponent<CameraComponent>();
        bool modified = false;
        float near = c.camera.GetNear();
        float far = c.camera.GetFar();

        PROPERTY_LABEL("Main Camera:");
        ImGui::Checkbox("##Main", &c.main);
        PROPERTY_LABEL("Clear color:");
        ImGui::ColorEdit4("##Clear color",&c.clearColor.r);

        PROPERTY_LABEL("Projection:");
        int selProj = c.camera.IsPerspective() ? 0 : 1;
        if(ImGui::Combo("##Projection", &selProj,"Perspective\0Orthographic\0\0")){
            if(selProj == 0)    c.camera.SetPerspective(1024.0f/720.0f,45.0f);
            else                c.camera.SetOrthographic(0.0f,1.0f,1.0f,0.0f);
        }

        if(c.camera.IsPerspective()){
            float fov = c.camera.GetFOV();
            float aspect = c.camera.GetAspect();
            PROPERTY_LABEL("FOV:");
            if(ImGui::SliderFloat("##FOV", &fov,10.0f,120.0f,"%.0f")) modified = true;
            PROPERTY_LABEL("Aspect:");
            if(ImGui::DragFloat("##Aspect", &aspect,0.1f,0.1f,FLT_MAX/INT_MAX)) modified = true;
            PROPERTY_LABEL("Clip Planes:");
            ImGui::Text("N:");
            ImGui::SameLine(115.0f);
            ImGui::SetNextItemWidth(55.0f);
            if(ImGui::DragFloat("##Near", &near,0.1f,0.01f,far-0.1f,"%.1f")) modified = true;
            ImGui::SameLine(180.0f);
            ImGui::Text("F:");
            ImGui::SameLine(195.0f);
            ImGui::SetNextItemWidth(55.0f);
            if(ImGui::DragFloat("##Far", &far,0.1f,near+0.1f ,FLT_MAX/INT_MAX,"%.1f")) modified = true;
            if(modified) c.camera.SetPerspective(aspect,fov,near,far);
        } else {
            float left = c.camera.GetLeft();
            float right = c.camera.GetRight();
            float top = c.camera.GetTop();
            float bottom = c.camera.GetBottom();
            PROPERTY_LABEL("Clip Planes:");
            ImGui::Text("T:");
            ImGui::SameLine(115.0f);
            ImGui::SetNextItemWidth(55.0f);
            if(ImGui::DragFloat("##Top", &top,0.1f,bottom+0.1f,FLT_MAX/INT_MAX,"%.1f")) modified = true;
            ImGui::SameLine(180.0f);
            ImGui::Text("B:");
            ImGui::SameLine(195.0f);
            ImGui::SetNextItemWidth(55.0f);
            if(ImGui::DragFloat("##Bottom", &bottom,0.1f,-FLT_MAX/INT_MAX ,top-0.1f,"%.1f")) modified = true;
            ImGui::NewLine();
            ImGui::SameLine(100.0f);
            ImGui::Text("L:");
            ImGui::SameLine(115.0f);
            ImGui::SetNextItemWidth(55.0f);
            if(ImGui::DragFloat("##Left", &left,0.1f,-FLT_MAX/INT_MAX,right-0.1f,"%.1f")) modified = true;
            ImGui::SameLine(180.0f);
            ImGui::Text("R:");
            ImGui::SameLine(195.0f);
            ImGui::SetNextItemWidth(55.0f);
            if(ImGui::DragFloat("##Right", &right,0.1f,left+0.1f ,FLT_MAX/INT_MAX,"%.1f")) modified = true;
            ImGui::NewLine();
            ImGui::SameLine(100.0f);
            ImGui::Text("N:");
            ImGui::SameLine(115.0f);
            ImGui::SetNextItemWidth(55.0f);
            if(ImGui::DragFloat("##Near", &near,0.1f,0.01f,far-0.1f,"%.1f")) modified = true;
            ImGui::SameLine(180.0f);
            ImGui::Text("F:");
            ImGui::SameLine(195.0f);
            ImGui::SetNextItemWidth(55.0f);
            if(ImGui::DragFloat("##Far", &far,0.1f,near+0.1f ,FLT_MAX/INT_MAX,"%.1f")) modified = true;
            if(modified) c.camera.SetOrthographic(left,right,top,bottom,near,far);
        }
    });
    m_EE.Register<MeshRendererComponent>("Mesh Renderer",[](Rush::Entity &e){
        auto &mi = e.GetComponent<MeshRendererComponent>();
        PROPERTY_LABEL("Mesh:");
        if(mi.mesh == nullptr)
            ImGui::Button("None");
        else
            ImGui::Button( mi.mesh->name.c_str());
        if(ImGui::BeginDragDropTarget()){
            const ImGuiPayload *meshPath = ImGui::AcceptDragDropPayload("mesh");
            if(meshPath != NULL)
                mi.mesh = AssetManager::GetMesh((const char *)meshPath->Data);
            ImGui::EndDragDropTarget();
        }
        if (mi.mesh) {
            for(auto &mesh : mi.mesh->submeshes){
                if(ImGui::TreeNode(mesh.meshName.c_str())){
                    ImGui::Text("Material:  ");
                    ImGui::SameLine();
                    if(mesh.material == nullptr)
                        ImGui::Button("None");
                    else
                        ImGui::Button( mesh.material->GetName().c_str());
                    if(ImGui::BeginDragDropTarget()){
                        const ImGuiPayload *matPath = ImGui::AcceptDragDropPayload("material");
                        if(matPath != NULL)
                            mesh.material = AssetManager::GetMaterial((const char *)matPath->Data);
                        ImGui::EndDragDropTarget();
                    }
                    ImGui::TreePop();
                }
            }
        }
    });
    m_EE.Register<LightComponent>("Light",[](Rush::Entity &e){
        auto& l = e.GetComponent<LightComponent>();
        PROPERTY_LABEL("Light Type:");
        int curType = (int)l.type;
        if(ImGui::Combo("##Light type",&curType,"Directional\0Point\0Spotlight\0\0")){
            l.type = (LightType)curType;
        }
        PROPERTY_LABEL("Ambient:");
        ImGui::ColorEdit3("##Ambient", &l.ambient.r);
        PROPERTY_LABEL("Diffuse:");
        ImGui::ColorEdit3("##Diffuse", &l.diffuse.r);
        PROPERTY_LABEL("Specular:");
        ImGui::ColorEdit3("##Specular", &l.specular.r);
        if(l.type != LightType::DIRECTIONAL){
            PROPERTY_LABEL("Range:");
            if(ImGui::DragFloat("##Range",&l.range,1.0f,0.01f,10000.0f) && !l.manualAtten){
                l.constant = 1.0f;
                l.linear = 4.5f / l.range;
                l.quadratic = 75.0f / (l.range * l.range);
            } 
            ImGui::Separator();
            ImGui::Text("Attenuation");
            PROPERTY_LABEL("Manual:");
            ImGui::Checkbox("##Manual Attenuation",&l.manualAtten);
            if(l.manualAtten) {
                PROPERTY_LABEL("Coefficients:");
                ImGui::DragFloat3("##Coefficients",&l.constant,0.01f,0.0f);
            }
        }
        if(l.type == LightType::SPOTLIGHT){
            PROPERTY_LABEL("Cutoff:");
            ImGui::DragFloat2("Cutoff",&l.cutOff);
        }
    });
    m_EE.Register<ParticleComponent>("Particle Emitter",[](Rush::Entity &e){
        auto &pe = e.GetComponent<ParticleComponent>();
        ParticleProperties &pp = pe.emissionProperties;
        ImGui::DragFloat("Emission rate", &pe.emissionRate,0.1f);
        ImGui::DragFloat3("Position",&pp.position.x,0.1f);
        ImGui::DragFloat3("Position variance",&pp.positionVariance.x,0.1f);
        ImGui::DragFloat3("Velocity",&pp.velocity.x,0.1f);
        ImGui::DragFloat3("Velocity variance",&pp.velocityVariance.x,0.1f);
        ImGui::DragFloat("Rotation",&pp.rotation);
        ImGui::DragFloat("Rotation variance",&pp.rotationVariance);
        ImGui::DragFloat("Angular velocity",&pp.angularVelocity);
        ImGui::DragFloat("Angular velocity variance",&pp.AVVariance);
        ImGui::DragFloat("Start size", &pp.sizeBegin,0.1f);
        ImGui::DragFloat("End size", &pp.sizeEnd,0.1f);
        ImGui::DragFloat("Size variance", &pp.sizeVariance,0.1f);
        ImGui::ColorEdit3("Start color", &pp.colorBegin.x);
        ImGui::ColorEdit3("End color", &pp.colorEnd.x);
        ImGui::DragFloat("Particle lifetime", &pp.lifetime,0.01f);
        ImGui::DragFloat("Lifetime Variance", &pp.lifetimeVariance,0.01f);
    });
    m_EEVisible = true;
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
    m_Focused = ImGui::IsWindowFocused();
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
    m_EE.Render(GlobalEntitySelection::GetSelection(),&m_EEVisible);
}

void SceneGraphView::RenderEntity(Rush::Entity e){
    std::string name;
    if(e.HasComponent<TagComponent>())
        name = e.GetComponent<TagComponent>().tag;
    if(name.empty())
        name = "Entity %d" + std::to_string(e.GetID());

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
        ImGui::TreeNodeEx(name.c_str(),flags);
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
        if(!e.HasComponent<TagComponent>())
            e.AddComponent<TagComponent>();
    
        TagComponent &name = e.GetComponent<TagComponent>();
        ImGui::SameLine();
        ImGui::InputText("##rename_field",&name.tag);
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
            ImGui::Button( mat.diffuseTexture->GetDebugName().c_str());

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
            ImGui::Button(mat.specularTexture->GetDebugName().c_str());

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
            ImGui::Button(mat.normalTexture->GetDebugName().c_str());

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
    if (!m_Focused) return false;
    Rush::Entity selection = GlobalEntitySelection::GetSelection();
    if(e.keycode == RUSH_KEY_DELETE && ((entt::entity)selection) != entt::null){
        //TODO: A bit hacky
        selection.GetParentRegistry()->destroy(selection);
        GlobalEntitySelection::ClearSelection();
        return true;
    }
    else if(e.keycode == RUSH_KEY_F2 && ((entt::entity)selection) != entt::null){
        m_Renaming = true;
        return true;
    }
    else if(e.keycode == RUSH_KEY_ENTER && m_Renaming){
        m_Renaming = false;
        return true;
    }
    return false;
}