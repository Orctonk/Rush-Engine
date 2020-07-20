#include "RenderViews.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

RenderViews::RenderViews()
:   m_CamController(Rush::Camera(Rush::ProjectionMode::PERSPECTIVE, 1024.0f/720.0f)) {
    for(int i = 0; i < RENDERVIEW_COUNT; i++)
        enabledViews[i] = true;     // TODO: Make window open status persistent over application close
}

RenderViews::~RenderViews(){

}

void RenderViews::Init(){
    using namespace Rush;

    m_CamController.GetCamera().SetPosition(glm::vec3(0.0f,0.0f,3.0f));
	m_CamController.GetCamera().SetRotation(-90.0f,0.0f,0.0f);

    m_LightBoxShader = AssetManager::GetShader("res/lightBoxShader.glsl");
    m_MaterialShader = AssetManager::GetShader("res/materialShader.glsl");
    int i = 0;
    m_MaterialShader->SetUniform("u_Material.diffuse",ShaderData::INT,&i);
    i = 1;
    m_MaterialShader->SetUniform("u_Material.specular",ShaderData::INT,&i);
    i = 2;
    m_MaterialShader->SetUniform("u_Material.normal",ShaderData::INT,&i);

    m_RenderViewShaders[RENDERVIEW_RENDER] = AssetManager::GetShader("res/renderviewPreview.glsl");
    m_RenderViewShaders[RENDERVIEW_NORMALS] = AssetManager::GetShader("res/renderviewNormals.glsl");
    m_RenderViewShaders[RENDERVIEW_ALBEDO] = AssetManager::GetShader("res/renderviewAlbedo.glsl");
    m_RenderViewShaders[RENDERVIEW_SPECULAR] = AssetManager::GetShader("res/renderviewSpecular.glsl");

    m_GBuffer = Framebuffer::Create({
        Application::GetInstance().GetWindow()->GetWidth(),
        Application::GetInstance().GetWindow()->GetHeight(),
        {16,16,8}
    });
    for(int i = 0; i < RENDERVIEW_COUNT; i++){
        m_RenderViews[i] = Framebuffer::Create({
            Application::GetInstance().GetWindow()->GetWidth(),
            Application::GetInstance().GetWindow()->GetHeight(),
            {16}
        });
    }
}

void RenderViews::OnUpdate(Rush::Scene &scene){
    using namespace Rush;

    auto &cam = m_CamController.GetCamera();
    RenderGBuffer(scene, cam);
    FillRenderView(scene, cam);
    for(int i = 1; i < RENDERVIEW_COUNT; i++)
        PopulateView((RenderView)i);
    
}

void RenderViews::OnEvent(Rush::Event &e){
    m_CamController.OnEvent(e);
}

void RenderViews::OnImguiRender(){
    bool resize = false;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f));
    if(enabledViews[RENDERVIEW_RENDER]){
        ImGui::Begin("Render",&enabledViews[RENDERVIEW_RENDER]);
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        glm::vec2 glmWinSize(windowSize.x,windowSize.y);
        float renderAspect = glmWinSize.x / glmWinSize.y;
        if(glmWinSize != m_RenderViewportSize){
            m_CamController.GetCamera().SetProjection(Rush::ProjectionMode::PERSPECTIVE,renderAspect);
            m_GBuffer->Resize((uint32_t)glmWinSize.x,(uint32_t)glmWinSize.y);
            for(int i = 0; i < RENDERVIEW_COUNT; i++){
                m_RenderViews[i]->Resize((uint32_t)glmWinSize.x,(uint32_t)glmWinSize.y);
            }
            resize = true;
            m_RenderViewportSize = glmWinSize;
        }
        ImGui::End();
    }
    
    RenderImguiView("Render", RENDERVIEW_RENDER,resize);
    RenderImguiView("Specular", RENDERVIEW_SPECULAR,resize);
    RenderImguiView("Albedo", RENDERVIEW_ALBEDO,resize);
    RenderImguiView("Normal", RENDERVIEW_NORMALS,resize);
    ImGui::PopStyleVar();
}

void RenderViews::RenderGBuffer(Rush::Scene &scene, Rush::Camera &cam){
    using namespace Rush;
    m_GBuffer->Bind();
    Renderer::GetAPI()->Clear();
    Renderer::BeginScene(cam);
    for(auto e : scene.View<MeshInstance>()){
        if(!scene.Has<Transform>(e))
            scene.Add<Transform>(e);
        auto &t = scene.Get<Transform>(e);
        MeshInstance &mesh = scene.Get<MeshInstance>(e);
        glm::mat4 model = glm::eulerAngleXYZ(glm::radians(t.rotation.x),glm::radians(t.rotation.y),glm::radians(t.rotation.z));
        model = glm::translate(glm::mat4(1.0f),t.translation) * model;
        model = glm::scale(model,t.scale);

        m_MaterialShader->Bind();
        for(auto &m : mesh.mesh->submeshes){
            m.material->diffuseTexture->Bind(0);
            m.material->specularTexture->Bind(1);
            m.material->normalTexture->Bind(2);
            Renderer::Submit(m_MaterialShader,m.vertices,model);
        }
    }

    Renderer::EndScene();
    m_GBuffer->Unbind();
}

void RenderViews::FillRenderView(Rush::Scene &scene, Rush::Camera &cam){
    using namespace Rush;
    m_RenderViews[RENDERVIEW_RENDER]->Bind();
    Renderer::GetAPI()->Clear();

    m_RenderViewShaders[RENDERVIEW_RENDER]->Bind();
    const char *uniforms[] = {
        "gPos",
        "gNorm",
        "gColor"
    };
    std::vector<Shared<Texture>> &gBufTextures = m_GBuffer->GetTextures();
    for(int i = 0; i < gBufTextures.size(); i++){
        gBufTextures.at(i)->Bind(i);
        m_RenderViewShaders[RENDERVIEW_RENDER]->SetUniform(uniforms[i],ShaderData::INT,&i);
    }
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

    m_RenderViewShaders[RENDERVIEW_RENDER]->SetUniform("u_CamPos",ShaderData::FLOAT3,glm::value_ptr(cam.GetPosition()));
    Renderer::RenderTexturedQuad(m_RenderViewShaders[RENDERVIEW_RENDER],{0.0f,0.0f},{1.0f,1.0f});
    
    m_GBuffer->Blit(m_RenderViews[RENDERVIEW_RENDER]);

    Renderer::BeginScene(cam);
    for(auto e: scene.View<PointLight>()){
        PointLight &l = scene.Get<PointLight>(e);
        glm::mat4 model = glm::translate(glm::mat4(1.0f),l.position);
        model = glm::scale(model,glm::vec3(0.1f));
        glm::vec3 avgColor = (l.ambient + l.diffuse + l.specular)/3.0f;
        m_LightBoxShader->SetUniform("u_LightCol",ShaderData::FLOAT3,glm::value_ptr(avgColor));
        Renderer::RenderCube(m_LightBoxShader,model);
    }
    Renderer::EndScene();
    m_RenderViews[RENDERVIEW_RENDER]->Unbind();
}

void RenderViews::PopulateView(RenderView type){
    using namespace Rush;
    m_RenderViewShaders[type]->Bind();
    const char *uniforms[] = {
        "gPos",
        "gNorm",
        "gColor"
    };
    std::vector<Shared<Texture>> &gBufTextures = m_GBuffer->GetTextures();
    for(int i = 0; i < gBufTextures.size(); i++){
        gBufTextures.at(i)->Bind(i);
        m_RenderViewShaders[type]->SetUniform(uniforms[i],ShaderData::INT,&i);
    }

    m_RenderViews[type]->Bind();
    Renderer::GetAPI()->Clear();
    Renderer::RenderTexturedQuad(m_RenderViewShaders[type],{0.0f,0.0f},{1.0f,1.0f});
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