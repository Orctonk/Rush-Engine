#include "RenderViews.h"

#include "GlobalEntitySelection.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

RenderViews::RenderViews()
:   m_CamController() {
    m_ObjectPick = false;
    m_GizmoOp = ImGuizmo::TRANSLATE;
    m_GizmoMode = ImGuizmo::WORLD;
    m_UsingGizmo = false;
    m_CurrentView = RenderView::RENDERVIEW_RENDER;
    for(int i = 0; i < RENDERVIEW_COUNT; i++)
        enabled = true;     // TODO: Make window open status persistent over application close
}

RenderViews::~RenderViews(){
    
}

void RenderViews::Init(Rush::Entity cameraEntity){
    using namespace Rush;

    m_CamController.SetControlledCamera(cameraEntity);

    cameraEntity.GetComponent<CameraComponent>().skybox = AssetManager::GetCubemap("res/skybox");

    m_SpotlightTexture = AssetManager::GetTexture("res/textures/lightbulb.png");
    m_DirlightTexture = AssetManager::GetTexture("res/textures/directional.png");
    m_SelectionShader = AssetManager::GetShader("res/shaders/selectionShader.glsl");
    m_SkyboxShader = AssetManager::GetShader("res/shaders/skyboxShader.glsl");
    int j = 0;
    m_SkyboxShader->SetUniform("u_Skybox",Rush::ShaderData::INT,&j);
    m_CameraMesh = AssetManager::GetMeshInstance("res/models/camera.obj");

    m_RenderViewShaders[RENDERVIEW_RENDER] = AssetManager::GetShader("res/shaders/renderviewPreview.glsl");
    m_RenderViewShaders[RENDERVIEW_NORMALS] = AssetManager::GetShader("res/shaders/renderviewNormals.glsl");
    m_RenderViewShaders[RENDERVIEW_ALBEDO] = AssetManager::GetShader("res/shaders/renderviewAlbedo.glsl");
    m_RenderViewShaders[RENDERVIEW_SPECULAR] = AssetManager::GetShader("res/shaders/renderviewSpecular.glsl");
    for(int i = 0; i < RENDERVIEW_COUNT; i++){
        j = 0;
        m_RenderViewShaders[i]->SetUniform("u_Material.diffuse",ShaderData::INT,&j);
        j = 1;
        m_RenderViewShaders[i]->SetUniform("u_Material.specular",ShaderData::INT,&j);
        j = 2;
        m_RenderViewShaders[i]->SetUniform("u_Material.normal",ShaderData::INT,&j);
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

    m_RenderView = Framebuffer::Create({
        Application::GetInstance().GetWindow()->GetWidth(),
        Application::GetInstance().GetWindow()->GetHeight(),
        {16}
    });

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
}

void RenderViews::OnEvent(Rush::Event &e){
    auto [x,y] = Rush::Input::MousePos();
    if(     x < m_RenderViewportPos.x || y < m_RenderViewportPos.y 
        ||  x > m_RenderViewportPos.x + m_RenderViewportSize.x
        ||  y > m_RenderViewportPos.y + m_RenderViewportSize.y)
        return;
    m_CamController.OnEvent(e);
    e.Dispatch<Rush::MouseReleasedEvent>(RUSH_BIND_FN(RenderViews::MouseClickHandle));
    e.Dispatch<Rush::KeyboardPressEvent>(RUSH_BIND_FN(RenderViews::KeyPressHandle));
    e.Dispatch<Rush::KeyboardReleaseEvent>(RUSH_BIND_FN(RenderViews::KeyReleaseHandle));
}

void RenderViews::OnImguiRender(){
    bool resize = false;
    ImGuizmo::BeginFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f));
    if(enabled){
        if(ImGui::Begin("Preview",&enabled)){
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();
            m_RenderViewportPos.x = windowPos.x;
            m_RenderViewportPos.y = windowPos.y;
            glm::vec2 glmWinSize(windowSize.x,windowSize.y);
            float renderAspect = glmWinSize.x / glmWinSize.y;
            if(glmWinSize != m_RenderViewportSize){
                m_CamController.GetCamera().GetComponent<CameraComponent>().camera.SetPerspective(renderAspect,90.0f);
                m_RenderView->Resize((uint32_t)glmWinSize.x,(uint32_t)glmWinSize.y);
                m_SelectionBuffer->Resize((uint32_t)glmWinSize.x,(uint32_t)glmWinSize.y);
                resize = true;
                m_RenderViewportSize = glmWinSize;
            }
            RenderImguiView(resize);
        }
        ImGui::End();
    }
    
    ImGui::PopStyleVar();
}

void RenderViews::FillRenderView(Rush::Scene &scene){
    using namespace Rush;
    m_RenderView->Bind();
    auto &cam = m_CamController.GetCamera().GetComponent<CameraComponent>();
    auto &camTrans = m_CamController.GetCamera().GetComponent<TransformComponent>();
    Renderer::GetAPI()->SetClearColor(cam.clearColor);
    Renderer::GetAPI()->Clear();
    glm::mat4 view = camTrans.GetModelMatrix();

    LineRenderer::BeginScene(cam.camera.GetProjection(),glm::inverse(camTrans.GetModelMatrix()));
    LineRenderer::GetAPI()->SetLineWidth(1.0f);
    for(float f = -10.0f; f < 10.5f; f++){
        LineRenderer::DrawLine({f,0.0f,-11.0f},{f,0.0f,11.0f},glm::vec4(.6f));
        LineRenderer::DrawLine({-11.0f,0.0f,f},{11.0f,0.0f,f},glm::vec4(.6f));
    }
    LineRenderer::EndScene();

    Renderer::BeginScene(cam.camera,view);
    auto reg = scene.GetRegistry();

    for(auto &e : reg->group<TransformComponent>(entt::get_t<MeshInstance>())){
        auto [transform, mesh] = reg->get<TransformComponent,MeshInstance>(e);
        glm::mat4 model = transform.GetModelMatrix();
        for(int i = 0; i < mesh.mesh->submeshes.size(); i++){
            mesh.mesh->submeshes[i].material.parent->diffuseTexture->Bind(0);
            mesh.mesh->submeshes[i].material.parent->specularTexture->Bind(1);
            mesh.mesh->submeshes[i].material.parent->normalTexture->Bind(2);
            Renderer::Submit(m_RenderViewShaders[m_CurrentView],mesh.mesh->submeshes[i].vertices,model);
        }
    }

    if(cam.skybox != nullptr){
        cam.skybox->Bind(0);
        Renderer::RenderCube(m_SkyboxShader,glm::mat4(1.0f));
    }

    if(m_CurrentView == RENDERVIEW_RENDER){
        for(auto e: reg->view<CameraComponent>()){
            if(e == m_CamController.GetCamera()) continue;
            auto [c,t] = reg->get<CameraComponent,TransformComponent>(e);
            glm::mat4 model = t.GetModelMatrix();
            for(int i = 0; i < m_CameraMesh.mesh->submeshes.size(); i++){
                m_CameraMesh.mesh->submeshes[i].material.parent->diffuseTexture->Bind(0);
                m_CameraMesh.mesh->submeshes[i].material.parent->specularTexture->Bind(1);
                m_CameraMesh.mesh->submeshes[i].material.parent->normalTexture->Bind(2);
                Renderer::Submit(m_RenderViewShaders[RENDERVIEW_RENDER],m_CameraMesh.mesh->submeshes[i].vertices,model);
            }
        }

        Renderer2D::BeginScene(cam.camera.GetProjection(),glm::inverse(camTrans.GetModelMatrix()));
        LineRenderer::BeginScene(cam.camera.GetProjection(),glm::inverse(camTrans.GetModelMatrix()));
        LineRenderer::GetAPI()->SetLineWidth(2.0f);
        for(auto e: reg->view<LightComponent>()){
            auto [l,t] = reg->get<LightComponent,TransformComponent>(e);
            glm::vec3 avgColor = (l.ambient + l.diffuse + l.specular)/3.0f;
            if(l.type == LightType::POINT)
                Renderer2D::DrawBillboard(t.GetTranslation(),{.2f,.2f},m_SpotlightTexture,glm::vec4(avgColor,1.0f));
            else if(l.type == LightType::DIRECTIONAL){
                Renderer2D::DrawBillboard(t.GetTranslation(),{.2f,.2f},m_DirlightTexture,glm::vec4(1.0f));
                glm::vec3 direction = glm::vec3(t.GetRotation() * glm::vec4(0.0f,0.0f,0.5f,0.0f));
                glm::vec3 up = glm::vec3(t.GetRotation() * glm::vec4(0.0f,1.0f,0.0f,0.0f));
                glm::vec3 right = glm::vec3(t.GetRotation() * glm::vec4(1.0f,0.0f,0.0f,0.0f));
                glm::vec3 pos = t.GetTranslation() + 0.1f * up;
                LineRenderer::DrawLine(pos,pos + direction,glm::vec4(avgColor,1.0f));
                pos -= 0.2f * up;
                LineRenderer::DrawLine(pos,pos + direction,glm::vec4(avgColor,1.0f));
                pos += 0.1f * up + 0.1f * right;
                LineRenderer::DrawLine(pos,pos + direction,glm::vec4(avgColor,1.0f));
                pos -= 0.2f * right;
                LineRenderer::DrawLine(pos,pos + direction,glm::vec4(avgColor,1.0f));
            }
        }

        LineRenderer::EndScene();
        Renderer2D::EndScene();
    }

    Renderer::EndScene();
    m_RenderView->Unbind();
}

void RenderViews::RenderImguiView(bool resized){
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    if(!resized){
        ImGui::GetWindowDrawList()->AddImage(
            (void *)m_RenderView->GetTextures().at(0)->GetID(),
            ImVec2(windowPos.x,windowPos.y),
            ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
            ImVec2(0,1),
            ImVec2(1,0)
        );
    }
    ImGui::SetNextItemWidth(100);
    ImGui::Combo("##CurrentView",(int*)&m_CurrentView,"Render\0Normal\0Albedo \0Specular\0\0");
    auto e = GlobalEntitySelection::GetSelection();
    if(e && e.HasComponent<TransformComponent>()){
        auto &c = m_CamController.GetCamera().GetComponent<CameraComponent>();
        auto &ct = m_CamController.GetCamera().GetComponent<TransformComponent>();
        auto &t = e.GetComponent<TransformComponent>();
        ImGuizmo::SetRect(windowPos.x,windowPos.y,windowSize.x,windowSize.y);
        ImGuizmo::SetDrawlist();
        glm::mat4 model = t.GetModelMatrix();
        glm::mat4 delta;
        ImGuizmo::Manipulate(&glm::inverse(ct.GetModelMatrix())[0].x, &c.camera.GetProjection()[0].x, m_GizmoOp, m_GizmoMode, &model[0].x, &delta[0].x);
        m_UsingGizmo = ImGuizmo::IsOver();
        if(ImGuizmo::IsUsing()){
            glm::quat orient;
            glm::vec3 trans,rot,scale,skew;
            glm::vec4 persp;
            glm::decompose(model,scale,orient,trans,skew,persp);
            switch(m_GizmoOp){
                case ImGuizmo::ROTATE:
                    t.SetRotation(orient);    
                    break;
                case ImGuizmo::TRANSLATE:
                    t.SetTranslation(trans); 
                    break;
                case ImGuizmo::SCALE:
                    t.SetScale(scale);
                    break;
            }
        }
    }
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
    glm::mat4 view = camTrans.GetModelMatrix();

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
        glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(t.GetModelMatrix() * glm::vec4(0.0f,0.0f,0.0f,1.0f)));
        model = glm::scale(model,glm::vec3(0.2f));
        Renderer::RenderCube(m_SelectionShader,model);  // Fake until model specific render state is supported
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

bool RenderViews::MouseClickHandle(Rush::MouseReleasedEvent &e){
    if(e.keycode != RUSH_MOUSE_BUTTON_LEFT || m_UsingGizmo) return false;
    
    m_ObjectPick = true;
    return true;
}

bool RenderViews::KeyPressHandle(Rush::KeyboardPressEvent &e){
    if(e.modifiers != RUSH_MOD_CONTROL) return false;

    switch(e.keycode){
        case RUSH_KEY_T:
            m_GizmoOp = ImGuizmo::TRANSLATE;
            return true;
        case RUSH_KEY_R:
            m_GizmoOp = ImGuizmo::ROTATE;
            return true;
        case RUSH_KEY_S:
            m_GizmoOp = ImGuizmo::SCALE;
            return true;
        case RUSH_KEY_L:
            m_GizmoMode = m_GizmoMode == ImGuizmo::WORLD ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
            return true;
        default:
            return false;
    }
}

bool RenderViews::KeyReleaseHandle(Rush::KeyboardReleaseEvent &e){
    return false;
}
