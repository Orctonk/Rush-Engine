#include "EditorLayer.h"

#include "EditorComponents.h"

#include "Rush/events/KeyboardEvent.h"
#include "Rush/events/Keycodes.h"
#include "Rush/graphics/VertexArray.h"
#include "Rush/graphics/Renderer.h"
#include "Rush/scene/Mesh.h"
#include "Rush/scene/Transform.h"
#include "Rush/scene/Light.h"

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 2, 3, 4, 5,    // Back face
	6, 7, 8, 9, 10, 11,    // Left face
	12, 13, 14, 15, 16, 17,    // Back face
	18, 19, 20, 21, 22, 23,    // Top face
	24, 25, 26, 27, 28, 29,    // Bottom face
	30, 31, 32, 33, 34, 35    // Front face
}; 

EditorLayer::EditorLayer() : m_EditorCamera(Rush::ProjectionMode::PERSPECTIVE, 1024.0f/720.0f) {}
EditorLayer::~EditorLayer() {}

void EditorLayer::OnAttach() {
    using namespace Rush;
    m_LightBoxShader = Shader::Create("res/lightBoxShader.glsl");
    m_LightingShader = Shader::Create("res/lightingPass.glsl");
    m_MeshShader = Shader::Create("res/geometryPass.glsl");

    m_Texture = Texture::Create("res/container.jpg");
    m_GBuffer = Framebuffer::Create({
        Application::GetInstance().GetWindow()->GetWidth(),
        Application::GetInstance().GetWindow()->GetHeight(),
        {16,16,8}
    });
    
    m_GBuffer->Unbind();

    m_EditorCamera.SetPosition(glm::vec3(0.0f,1.0f,3.0f));
	m_EditorCamera.SetRotation(90.0f,20.0f,0.0f);

    auto &reg = Application::GetInstance().GetRegistry();

    Shared<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));
    vb->SetInterleavedLayout({
        Rush::ShaderData::FLOAT3,
        Rush::ShaderData::FLOAT3,
        Rush::ShaderData::FLOAT2
    });
    Shared<IndexBuffer> ib = IndexBuffer::Create(indices, 36);

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

    auto box = reg.create();
    auto &m = reg.emplace<Mesh>(box,VertexArray::Create());
    m.vertices->AddVertexBuffer(vb);
    m.vertices->SetIndexBuffer(ib);

    reg.emplace<Transform>(box,glm::vec3(0.0f),glm::vec3(0.0f));
    reg.emplace<EntityName>(box, "Mesh Test");

    m_SelectedEnt = entt::null;
    m_EE.Register<Transform>("Transform",[](entt::registry &reg,entt::entity e){
        auto& t = reg.get<Transform>(e);
        ImGui::DragFloat3("Position", &t.translation.x, 0.01f);
        ImGui::DragFloat3("Rotation", &t.rotation.x, 1.0f);
        ImGui::DragFloat3("Scale", &t.scale.x, 0.01f);
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
        ImGui::DragFloat3("Coef.", &l.constant,0.01f);
    });
}
void EditorLayer::OnDetach() {}
void EditorLayer::OnUpdate() {
    using namespace Rush;
    auto &reg = Application::GetInstance().GetRegistry();
    m_GBuffer->Bind();
    Renderer::GetAPI()->Clear();
    m_Texture->Bind(0);
    Renderer::BeginScene(m_EditorCamera);
    for(auto e : reg.view<Mesh>()){
        Transform &t = reg.get_or_emplace<Transform>(e);
        Mesh &m = reg.get<Mesh>(e);
        glm::mat4 model = glm::eulerAngleXYZ(glm::radians(t.rotation.x),glm::radians(t.rotation.y),glm::radians(t.rotation.z));
        model = glm::translate(glm::mat4(1.0f),t.translation) * model;
        model = glm::scale(model,t.scale);

        Renderer::Submit(m_MeshShader,m.vertices,model);
    }
    Renderer::EndScene();
    m_GBuffer->Unbind();

    Renderer::GetAPI()->Clear();

    m_LightingShader->Bind();
    const char *uniforms[] = {
        "gPos",
        "gNorm",
        "gColor"
    };
    std::vector<Shared<Texture>> &gBufTextures = m_GBuffer->GetTextures();
    for(int i = 0; i < gBufTextures.size(); i++){
        gBufTextures.at(i)->Bind(i);
        m_LightingShader->SetUniform(uniforms[i],ShaderData::INT,&i);
    }
    bool activated = true;
    uint8_t lightCount = 0;
    for(auto e : reg.view<DirectionalLight>()){
        DirectionalLight &l = reg.get<DirectionalLight>(e);
        m_LightingShader->SetUniform("u_DLights[" + std::to_string(lightCount) + "].activated",ShaderData::BOOL,&activated);
        m_LightingShader->SetUniform("u_DLights[" + std::to_string(lightCount) + "].direction",ShaderData::FLOAT3,glm::value_ptr(l.direction));
        m_LightingShader->SetUniform("u_DLights[" + std::to_string(lightCount) + "].ambient",ShaderData::FLOAT3,glm::value_ptr(l.ambient));
        m_LightingShader->SetUniform("u_DLights[" + std::to_string(lightCount) + "].diffuse",ShaderData::FLOAT3,glm::value_ptr(l.diffuse));
        m_LightingShader->SetUniform("u_DLights[" + std::to_string(lightCount) + "].specular",ShaderData::FLOAT3,glm::value_ptr(l.specular));
        lightCount++;
        if(lightCount >= 5)
            break;
    }
    lightCount = 0;
    for(auto e : reg.view<PointLight>()){
        PointLight &l = reg.get<PointLight>(e);
        m_LightingShader->SetUniform("u_PLights[" + std::to_string(lightCount) + "].activated",ShaderData::BOOL,&activated);
        m_LightingShader->SetUniform("u_PLights[" + std::to_string(lightCount) + "].position",ShaderData::FLOAT3,glm::value_ptr(l.position));
        m_LightingShader->SetUniform("u_PLights[" + std::to_string(lightCount) + "].ambient",ShaderData::FLOAT3,glm::value_ptr(l.ambient));
        m_LightingShader->SetUniform("u_PLights[" + std::to_string(lightCount) + "].diffuse",ShaderData::FLOAT3,glm::value_ptr(l.diffuse));
        m_LightingShader->SetUniform("u_PLights[" + std::to_string(lightCount) + "].specular",ShaderData::FLOAT3,glm::value_ptr(l.specular));
        m_LightingShader->SetUniform("u_PLights[" + std::to_string(lightCount) + "].constant",ShaderData::FLOAT,&l.constant);
        m_LightingShader->SetUniform("u_PLights[" + std::to_string(lightCount) + "].linear",ShaderData::FLOAT,&l.linear);
        m_LightingShader->SetUniform("u_PLights[" + std::to_string(lightCount) + "].quadratic",ShaderData::FLOAT,&l.quadratic);
        lightCount++;
        if(lightCount >= 10)
            break;
    }
    m_LightingShader->SetUniform("u_CamPos",ShaderData::FLOAT3,glm::value_ptr(m_EditorCamera.GetPosition()));
    Renderer::RenderTexturedQuad(m_LightingShader,{0.0f,0.0f},{1.0f,1.0f});
    
    Unique<Framebuffer> fb = nullptr;
    m_GBuffer->Blit(fb);

    Renderer::BeginScene(m_EditorCamera);
    for(auto e: reg.view<PointLight>()){
        PointLight &l = reg.get<PointLight>(e);
        glm::mat4 model = glm::translate(glm::mat4(1.0f),l.position);
        model = glm::scale(model,glm::vec3(0.1f));
        glm::vec3 avgColor = (l.ambient + l.diffuse + l.specular)/3.0f;
        m_LightBoxShader->SetUniform("u_LightCol",ShaderData::FLOAT3,glm::value_ptr(avgColor));
        Renderer::RenderCube(m_LightBoxShader,model);
    }
    Renderer::EndScene();
}

void EditorLayer::OnEvent(Rush::Event &e) {
    e.Dispatch<Rush::KeyboardPressEvent>(RUSH_BIND_FN(EditorLayer::KeyPressHandle));
}
void EditorLayer::OnImguiRender() {
    entt::registry &reg = Rush::Application::GetInstance().GetRegistry();

    ImGui::Begin("Scene Editor");
    ImGui::Text("Scene graph");
    float width = ImGui::GetWindowWidth();
    ImGui::SameLine(width - 50.0f);
    if(ImGui::Button("+")) reg.create();
    ImGui::SameLine();
    if(ImGui::Button("-") && m_SelectedEnt != entt::null) {
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