#include "RenderViews.h"

#include "GlobalEntitySelection.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

RenderViews::RenderViews()
:   m_CamController() {
    m_ObjectPick = false;
    for(int i = 0; i < RENDERVIEW_COUNT; i++)
        enabledViews[i] = true;     // TODO: Make window open status persistent over application close
}

RenderViews::~RenderViews(){

}

void RenderViews::Init(Rush::Entity cameraEntity){
    using namespace Rush;

    m_CamController.SetControlledCamera(cameraEntity);

    m_LightBoxShader = AssetManager::GetShader("res/lightBoxShader.glsl");
    m_SelectionShader = AssetManager::GetShader("res/selectionShader.glsl");
    m_CameraMesh = AssetManager::GetMeshInstance("res/camera.obj");

    m_RenderViewShaders[RENDERVIEW_RENDER] = AssetManager::GetShader("res/renderviewPreview.glsl");
    m_RenderViewShaders[RENDERVIEW_NORMALS] = AssetManager::GetShader("res/renderviewNormals.glsl");
    m_RenderViewShaders[RENDERVIEW_ALBEDO] = AssetManager::GetShader("res/renderviewAlbedo.glsl");
    m_RenderViewShaders[RENDERVIEW_SPECULAR] = AssetManager::GetShader("res/renderviewSpecular.glsl");
    for(int i = 0; i < RENDERVIEW_COUNT; i++){
        int j = 0;
        m_RenderViewShaders[i]->SetUniform("u_Material.diffuse",ShaderData::INT,&j);
        j = 1;
        m_RenderViewShaders[i]->SetUniform("u_Material.specular",ShaderData::INT,&j);
        j = 2;
        m_RenderViewShaders[i]->SetUniform("u_Material.normal",ShaderData::INT,&j);
    }

    for(int i = 0; i < RENDERVIEW_COUNT; i++){
        m_RenderViews[i] = Framebuffer::Create({
            Application::GetInstance().GetWindow()->GetWidth(),
            Application::GetInstance().GetWindow()->GetHeight(),
            {16}
        });
    }

    m_SelectionBuffer = Framebuffer::Create({
            Application::GetInstance().GetWindow()->GetWidth(),
            Application::GetInstance().GetWindow()->GetHeight(),
            {8}
        });
}

void RenderViews::OnUpdate(Rush::Scene &scene){
    using namespace Rush;
    if(m_ObjectPick)
        DoObjectPick(scene);

    auto &cam = m_CamController.GetCamera();
    FillRenderView(scene);
    for(int i = 1; i < RENDERVIEW_COUNT; i++)
        PopulateView(scene,(RenderView)i);
    
}

void RenderViews::OnEvent(Rush::Event &e){
    m_CamController.OnEvent(e);
    e.Dispatch<Rush::MousePressedEvent>(RUSH_BIND_FN(RenderViews::MouseClickHandle));
}

void RenderViews::OnImguiRender(){
    bool resize = false;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f));
    if(enabledViews[RENDERVIEW_RENDER]){
        ImGui::Begin("Render",&enabledViews[RENDERVIEW_RENDER]);
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        m_RenderViewportPos.x = windowPos.x;
        m_RenderViewportPos.y = windowPos.y;
        glm::vec2 glmWinSize(windowSize.x,windowSize.y);
        float renderAspect = glmWinSize.x / glmWinSize.y;
        if(glmWinSize != m_RenderViewportSize){
            m_CamController.GetCamera().GetComponent<CameraComponent>().camera.SetPerspective(renderAspect,90.0f);
            for(int i = 0; i < RENDERVIEW_COUNT; i++){
                m_RenderViews[i]->Resize((uint32_t)glmWinSize.x,(uint32_t)glmWinSize.y);
            }
            m_SelectionBuffer->Resize((uint32_t)glmWinSize.x,(uint32_t)glmWinSize.y);
            resize = true;
            m_RenderViewportSize = glmWinSize;
        }
        ImGui::End();
    }
    
    RenderImguiView("Render", RENDERVIEW_RENDER,resize);
    RenderImguiView("Normal", RENDERVIEW_NORMALS,resize);
    RenderImguiView("Albedo", RENDERVIEW_ALBEDO,resize);
    RenderImguiView("Specular", RENDERVIEW_SPECULAR,resize);
    ImGui::PopStyleVar();
}

void RenderViews::FillRenderView(Rush::Scene &scene){
    using namespace Rush;
    m_RenderViews[RENDERVIEW_RENDER]->Bind();
    Renderer::GetAPI()->Clear();
    auto &cam = m_CamController.GetCamera().GetComponent<CameraComponent>();
    auto &camTrans = m_CamController.GetCamera().GetComponent<TransformComponent>();
    glm::mat4 view = glm::yawPitchRoll(glm::radians(camTrans.rotation.y),glm::radians(camTrans.rotation.x),glm::radians(camTrans.rotation.z));
    view = glm::translate(glm::mat4(1.0f),camTrans.translation) * view;

    Renderer::BeginScene(cam.camera,view);

    glm::vec3 dlightcol(1.0f);
    glm::vec3 dlights[2];
    dlights[0] = glm::vec3(-1.0f);
    dlights[1] = glm::vec3(1.0f);
    glm::vec3 zero(0.0f);
    for(int i = 0; i < 2; i++){
        m_RenderViewShaders[RENDERVIEW_RENDER]->SetUniform("u_DLights[" + std::to_string(i) + "].direction",ShaderData::FLOAT3,glm::value_ptr(dlights[i]));
        m_RenderViewShaders[RENDERVIEW_RENDER]->SetUniform("u_DLights[" + std::to_string(i) + "].ambient",ShaderData::FLOAT3,glm::value_ptr(dlightcol * 0.1f));
        m_RenderViewShaders[RENDERVIEW_RENDER]->SetUniform("u_DLights[" + std::to_string(i) + "].diffuse",ShaderData::FLOAT3,glm::value_ptr(dlightcol));
        m_RenderViewShaders[RENDERVIEW_RENDER]->SetUniform("u_DLights[" + std::to_string(i) + "].specular",ShaderData::FLOAT3,glm::value_ptr(zero));
    }
    auto reg = scene.GetRegistry();
    for(auto &e : reg->group<TransformComponent>(entt::get_t<MeshInstance>())){
        auto [transform, mesh] = reg->get<TransformComponent,MeshInstance>(e);
        glm::mat4 model = glm::yawPitchRoll(glm::radians(transform.rotation.y),glm::radians(transform.rotation.x),glm::radians(transform.rotation.z));
        model = glm::translate(glm::mat4(1.0f),transform.translation) * model;
        model = glm::scale(model,transform.scale);
        for(int i = 0; i < mesh.mesh->submeshes.size(); i++){
            mesh.mesh->submeshes[i].material.parent->diffuseTexture->Bind(0);
            mesh.mesh->submeshes[i].material.parent->specularTexture->Bind(1);
            mesh.mesh->submeshes[i].material.parent->normalTexture->Bind(2);
            Renderer::Submit(m_RenderViewShaders[RENDERVIEW_RENDER],mesh.mesh->submeshes[i].vertices,model);
        }
    }
    
    for(auto e: reg->view<LightComponent>()){
        auto [l,t] = reg->get<LightComponent,TransformComponent>(e);
        glm::mat4 model = glm::translate(glm::mat4(1.0f),t.translation);
        model = glm::scale(model,glm::vec3(0.1f));
        glm::vec3 avgColor = (l.ambient + l.diffuse + l.specular)/3.0f;
        m_LightBoxShader->SetUniform("u_LightCol",ShaderData::FLOAT3,glm::value_ptr(avgColor));
        Renderer::RenderCube(m_LightBoxShader,model);
    }

    for(auto e: reg->view<CameraComponent>()){
        if(e == m_CamController.GetCamera()) continue;
        auto [c,t] = reg->get<CameraComponent,TransformComponent>(e);
        glm::mat4 model = glm::yawPitchRoll(glm::radians(t.rotation.y),glm::radians(t.rotation.x),glm::radians(t.rotation.z));
        model = glm::translate(glm::mat4(1.0f),t.translation) * model;
        for(int i = 0; i < m_CameraMesh.mesh->submeshes.size(); i++){
            m_CameraMesh.mesh->submeshes[i].material.parent->diffuseTexture->Bind(0);
            m_CameraMesh.mesh->submeshes[i].material.parent->specularTexture->Bind(1);
            m_CameraMesh.mesh->submeshes[i].material.parent->normalTexture->Bind(2);
            Renderer::Submit(m_RenderViewShaders[RENDERVIEW_RENDER],m_CameraMesh.mesh->submeshes[i].vertices,model);
        }
    }
    Renderer::EndScene();
    m_RenderViews[RENDERVIEW_RENDER]->Unbind();
}

void RenderViews::PopulateView(Rush::Scene &scene, RenderView type){
    using namespace Rush;

    m_RenderViews[type]->Bind();
    Renderer::GetAPI()->Clear();
    auto &cam = m_CamController.GetCamera().GetComponent<CameraComponent>();
    auto &camTrans = m_CamController.GetCamera().GetComponent<TransformComponent>();
    glm::mat4 view = glm::yawPitchRoll(glm::radians(camTrans.rotation.y),glm::radians(camTrans.rotation.x),glm::radians(camTrans.rotation.z));
    view = glm::translate(glm::mat4(1.0f),camTrans.translation) * view;

    Renderer::BeginScene(cam.camera,view);

    auto reg = scene.GetRegistry();
    for(auto &e : reg->group<TransformComponent>(entt::get_t<MeshInstance>())){
        auto [transform, mesh] = reg->get<TransformComponent,MeshInstance>(e);
        glm::mat4 model = glm::yawPitchRoll(glm::radians(transform.rotation.y),glm::radians(transform.rotation.x),glm::radians(transform.rotation.z));
        model = glm::translate(glm::mat4(1.0f),transform.translation) * model;
        model = glm::scale(model,transform.scale);
        for(int i = 0; i < mesh.mesh->submeshes.size(); i++){
            mesh.mesh->submeshes[i].material.parent->diffuseTexture->Bind(0);
            mesh.mesh->submeshes[i].material.parent->specularTexture->Bind(1);
            mesh.mesh->submeshes[i].material.parent->normalTexture->Bind(2);
            Renderer::Submit(m_RenderViewShaders[type],mesh.mesh->submeshes[i].vertices,model);
        }
    }
    Renderer::EndScene();
    m_RenderViews[type]->Unbind();
}

void RenderViews::RenderImguiView(const char *name, RenderView type, bool resized){
    float renderAspect = m_RenderViewportSize.x / m_RenderViewportSize.y;
    if(!enabledViews[type])
        return;

    ImGui::Begin(name,&enabledViews[type]);
    if(!resized){
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        float thisAspect = windowSize.x / windowSize.y;
        glm::vec2 offset;
        if((renderAspect - thisAspect) * (renderAspect - thisAspect) < 0.01)
            offset = {0.0f,0.0f};
        else if(renderAspect < thisAspect)
            offset = {(windowSize.x - windowSize.y * m_RenderViewportSize.x / m_RenderViewportSize.y)/2,0.0f};
        else if (renderAspect > thisAspect)
            offset = {0.0f, (windowSize.y - windowSize.x * m_RenderViewportSize.y / m_RenderViewportSize.x)/2};

        ImGui::GetWindowDrawList()->AddImage(
            (void *)m_RenderViews[type]->GetTextures().at(0)->GetID(),
            ImVec2(windowPos.x + offset.x,windowPos.y + offset.y),
            ImVec2(windowPos.x + windowSize.x - offset.x*2, windowPos.y + windowSize.y - offset.y*2),
            ImVec2(0,1),
            ImVec2(1,0)
        );
    }
    ImGui::End();
}

bool RenderViews::MouseClickHandle(Rush::MousePressedEvent &e){
    if(e.keycode != RUSH_MOUSE_BUTTON_LEFT) return false;
    
    m_ObjectPick = true;
    return true;
}

void RenderViews::DoObjectPick(Rush::Scene &scene){
    using namespace Rush;
    m_ObjectPick = false;
    auto [x,y] = Rush::Input::MousePos();
    auto p = m_SelectionBuffer->GetTextures().at(0)->GetProperties();
    glm::vec<2,int> texClickPos(x - m_RenderViewportPos.x,p.height - (y - m_RenderViewportPos.y));
    if( texClickPos.x < 0 || texClickPos.x >= p.width || 
        texClickPos.y < 0 || texClickPos.y >= p.height){
        return;
    }
    
    m_SelectionBuffer->Bind();
    glm::vec4 color = glm::vec4(1.0f);
    color.r = ((float)(entt::to_integral(entt::null) & 0x0000FF))/255.0f;
    color.g = ((float)((entt::to_integral(entt::null) >> 8) & 0x0000FF))/255.0f;
    color.b = ((float)((entt::to_integral(entt::null) >> 16) & 0x0000FF))/255.0f;
    color.a = ((float)((entt::to_integral(entt::null) >> 24) & 0x0000FF))/255.0f;
    Renderer::GetAPI()->SetClearColor(color);
    Renderer::GetAPI()->Clear();
    auto &cam = m_CamController.GetCamera().GetComponent<CameraComponent>();
    auto &camTrans = m_CamController.GetCamera().GetComponent<TransformComponent>();
    glm::mat4 view = glm::yawPitchRoll(glm::radians(camTrans.rotation.y),glm::radians(camTrans.rotation.x),glm::radians(camTrans.rotation.z));
    view = glm::translate(glm::mat4(1.0f),camTrans.translation) * view;

    Renderer::BeginScene(cam.camera,view);

    auto reg = scene.GetRegistry();
    for(auto &e : reg->group<TransformComponent>(entt::get_t<MeshInstance>())){
        color.r = ((float)(entt::to_integral(e) & 0x0000FF))/255.0f;
        color.g = ((float)((entt::to_integral(e) >> 8) & 0x0000FF))/255.0f;
        color.b = ((float)((entt::to_integral(e) >> 16) & 0x0000FF))/255.0f;
        color.a = ((float)((entt::to_integral(e) >> 24) & 0x0000FF))/255.0f;
        m_SelectionShader->SetUniform("u_IDColor", ShaderData::FLOAT4, glm::value_ptr(color));
        auto [transform, mesh] = reg->get<TransformComponent,MeshInstance>(e);
        glm::mat4 model = transform.GetModelMatrix();
        for(int i = 0; i < mesh.mesh->submeshes.size(); i++){
            Renderer::Submit(m_SelectionShader,mesh.mesh->submeshes[i].vertices,model);
        }
    }
    
    for(auto e: reg->view<LightComponent>()){
        color.r = ((float)(entt::to_integral(e) & 0x0000FF))/255.0f;
        color.g = ((float)((entt::to_integral(e) >> 8) & 0x0000FF))/255.0f;
        color.b = ((float)((entt::to_integral(e) >> 16) & 0x0000FF))/255.0f;
        color.a = ((float)((entt::to_integral(e) >> 24) & 0x0000FF))/255.0f;
        m_SelectionShader->SetUniform("u_IDColor", ShaderData::FLOAT4, glm::value_ptr(color));
        auto [l,t] = reg->get<LightComponent,TransformComponent>(e);
        glm::mat4 model = glm::translate(glm::mat4(1.0f),t.translation);
        model = glm::scale(model,glm::vec3(0.1f));
        Renderer::RenderCube(m_SelectionShader,model);
    }

    for(auto e: reg->view<CameraComponent>()){
        if(e == m_CamController.GetCamera()) continue;
        color.r = ((float)(entt::to_integral(e) & 0x0000FF))/255.0f;
        color.g = ((float)((entt::to_integral(e) >> 8) & 0x0000FF))/255.0f;
        color.b = ((float)((entt::to_integral(e) >> 16) & 0x0000FF))/255.0f;
        color.a = ((float)((entt::to_integral(e) >> 24) & 0x0000FF))/255.0f;
        m_SelectionShader->SetUniform("u_IDColor", ShaderData::FLOAT4, glm::value_ptr(color));
        auto [c,t] = reg->get<CameraComponent,TransformComponent>(e);
        glm::mat4 model = t.GetModelMatrix();
        for(int i = 0; i < m_CameraMesh.mesh->submeshes.size(); i++){
            Renderer::Submit(m_SelectionShader,m_CameraMesh.mesh->submeshes[i].vertices,model);
        }
    }
    Renderer::EndScene();
    m_SelectionBuffer->Unbind();
    int size = p.width * p.height * p.bpp/8 * p.channels;
    unsigned char *buffer = new unsigned char [size];

    m_SelectionBuffer->GetTextures().at(0)->GetTextureData(buffer,size);
    unsigned char r,g,b,a;

    int index = (texClickPos.x + p.width * texClickPos.y) * p.bpp/8 * p.channels;
    r = buffer[index];
    g = buffer[index + 1];
    b = buffer[index + 2];
    a = buffer[index + 3];
    delete[] buffer;
    uint32_t id = (r) | (g << 8) | (b << 16) | (a << 24);
    entt::entity picked = (entt::entity)id;

    GlobalEntitySelection::SetSelection({reg, picked});
}