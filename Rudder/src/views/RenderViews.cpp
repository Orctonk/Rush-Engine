#include "RenderViews.h"
#include "Rudderpch.h"

#include "GlobalEntitySelection.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

RenderViews::RenderViews()
    : m_CamController() {
    m_ObjectPick = false;
    m_GizmoOp = ImGuizmo::TRANSLATE;
    m_GizmoMode = ImGuizmo::WORLD;
    m_Focused = false;
    m_UsingGizmo = false;
    m_CurrentView = RenderView::RENDERVIEW_RENDER;
    for (int i = 0; i < RENDERVIEW_COUNT; i++)
        enabled = true; // TODO: Make window open status persistent over application close
}

RenderViews::~RenderViews() {
}

void RenderViews::Init(Rush::Entity cameraEntity) {
    using namespace Rush;

    m_CamController.SetControlledCamera(cameraEntity);

    m_SpotlightTexture = AssetManager::GetTexture("res/textures/lightbulb.png");
    m_DirlightTexture = AssetManager::GetTexture("res/textures/directional.png");
    m_SelectionShader = AssetManager::GetShader("res/shaders/selectionShader.glsl");
    m_CameraMesh = AssetManager::GetMesh("res/models/camera.obj");

    m_RenderViewShaders[RENDERVIEW_RENDER] = AssetManager::GetShader("res/shaders/renderviewPreview.glsl");
    m_RenderViewShaders[RENDERVIEW_NORMALS] = AssetManager::GetShader("res/shaders/renderviewNormals.glsl");
    m_RenderViewShaders[RENDERVIEW_ALBEDO] = AssetManager::GetShader("res/shaders/renderviewAlbedo.glsl");
    m_RenderViewShaders[RENDERVIEW_SPECULAR] = AssetManager::GetShader("res/shaders/renderviewSpecular.glsl");

    m_RenderView = Framebuffer::Create({ Application::GetInstance().GetWindow()->GetWidth(),
                                         Application::GetInstance().GetWindow()->GetHeight(),
                                         { TextureFormat::RGBA16F } });

    m_SelectionBuffer = Framebuffer::Create({ Application::GetInstance().GetWindow()->GetWidth(),
                                              Application::GetInstance().GetWindow()->GetHeight(),
                                              { TextureFormat::RGBA8 } });

    // Trigger resize on reinit
    m_RenderViewportSize = { 0.0f, 0.0f };
}

void RenderViews::OnUpdate(Rush::Scene &scene) {
    using namespace Rush;
    if (m_ObjectPick)
        DoObjectPick(scene);

    auto &cam = m_CamController.GetCamera();
    FillRenderView(scene);
}

void RenderViews::OnEvent(Rush::Event &e) {
    m_CamController.OnEvent(e);
    e.Dispatch<Rush::MouseReleasedEvent>(RUSH_BIND_FN(RenderViews::MouseClickHandle));
    e.Dispatch<Rush::KeyboardPressEvent>(RUSH_BIND_FN(RenderViews::KeyPressHandle));
    e.Dispatch<Rush::KeyboardReleaseEvent>(RUSH_BIND_FN(RenderViews::KeyReleaseHandle));
}

void RenderViews::OnImguiRender() {
    bool resize = false;
    ImGuizmo::BeginFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (enabled) {
        if (ImGui::Begin("Preview", &enabled)) {
            m_Focused = ImGui::IsWindowFocused();
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 min = ImGui::GetWindowContentRegionMin();
            ImVec2 max = ImGui::GetWindowContentRegionMax();
            m_RenderViewportPos.x = windowPos.x + min.x;
            m_RenderViewportPos.y = windowPos.y + min.y;
            glm::vec2 glmWinSize(max.x - min.x, max.y - min.y);
            float renderAspect = glmWinSize.x / glmWinSize.y;
            if (glmWinSize != m_RenderViewportSize) {
                m_CamController.GetCamera().GetComponent<CameraComponent>().camera.SetPerspective(renderAspect, 90.0f);
                m_RenderView->Resize((uint32_t)glmWinSize.x, (uint32_t)glmWinSize.y);
                m_SelectionBuffer->Resize((uint32_t)glmWinSize.x, (uint32_t)glmWinSize.y);
                resize = true;
                m_RenderViewportSize = glmWinSize;
            }
            RenderImguiView(resize);
        }
        m_Focused = ImGui::IsWindowFocused();
        ImGui::End();
    }
    ImGui::PopStyleVar();
}

void RenderViews::FillRenderView(Rush::Scene &scene) {
    using namespace Rush;
    m_RenderView->Bind();
    auto &cam = m_CamController.GetCamera().GetComponent<CameraComponent>();
    auto &camTrans = m_CamController.GetCamera().GetComponent<TransformComponent>();
    Renderer::GetAPI()->SetClearColor(cam.clearColor);
    Renderer::GetAPI()->Clear();
    glm::mat4 view = camTrans.GetModelMatrix();

    auto reg = scene.GetRegistry();
    if (m_CurrentView == RENDERVIEW_RENDER)
        scene.Render();
    else {
        Renderer::BeginScene(cam.camera, view, m_RenderViewShaders[m_CurrentView]);

        for (auto &e : reg->group<TransformComponent>(entt::get_t<MeshRendererComponent>())) {
            auto [transform, mesh] = reg->get<TransformComponent, MeshRendererComponent>(e);
            glm::mat4 model = transform.GetModelMatrix();
            if (mesh.mesh) {
                for (auto &sm : mesh.mesh->submeshes) {
                    Renderer::Submit(sm.material, sm.vertices, model);
                }
            }
        }

        Renderer::EndScene();
    }

    LineRenderer::BeginScene(cam.camera.GetProjection(), glm::inverse(view));
    LineRenderer::GetAPI()->SetLineWidth(2.0f);
    for (float f = -10.0f; f < 10.5f; f++) {
        LineRenderer::DrawLine({ f, 0.0f, -11.0f }, { f, 0.0f, 11.0f }, glm::vec4(.6f));
        LineRenderer::DrawLine({ -11.0f, 0.0f, f }, { 11.0f, 0.0f, f }, glm::vec4(.6f));
    }
    LineRenderer::EndScene();

    if (m_CurrentView == RENDERVIEW_RENDER) { // TODO: Fix editor not drawing billboards that are behind transparent objects
        Renderer2D::BeginScene(cam.camera.GetProjection(), glm::inverse(view));
        LineRenderer::BeginScene(cam.camera.GetProjection(), glm::inverse(view));
        LineRenderer::GetAPI()->SetLineWidth(2.0f);
        for (auto e : reg->view<LightComponent>()) {
            auto [l, t] = reg->get<LightComponent, TransformComponent>(e);
            glm::vec3 avgColor = (l.ambient + l.diffuse + l.specular) / 3.0f;
            if (l.type == LightType::POINT)
                Renderer2D::DrawBillboard(t.GetTranslation(), { .2f, .2f }, m_SpotlightTexture, glm::vec4(avgColor, 1.0f));
            else if (l.type == LightType::DIRECTIONAL) {
                Renderer2D::DrawBillboard(t.GetTranslation(), { .2f, .2f }, m_DirlightTexture, glm::vec4(1.0f));
                glm::vec3 direction = glm::vec3(t.GetRotation() * glm::vec4(0.0f, 0.0f, 0.5f, 0.0f));
                glm::vec3 up = glm::vec3(t.GetRotation() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                glm::vec3 right = glm::vec3(t.GetRotation() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
                glm::vec3 pos = t.GetTranslation() + 0.1f * up;
                LineRenderer::DrawLine(pos, pos + direction, glm::vec4(avgColor, 1.0f));
                pos -= 0.2f * up;
                LineRenderer::DrawLine(pos, pos + direction, glm::vec4(avgColor, 1.0f));
                pos += 0.1f * up + 0.1f * right;
                LineRenderer::DrawLine(pos, pos + direction, glm::vec4(avgColor, 1.0f));
                pos -= 0.2f * right;
                LineRenderer::DrawLine(pos, pos + direction, glm::vec4(avgColor, 1.0f));
            }
        }

        LineRenderer::EndScene();
        Renderer2D::EndScene();
    }

    m_RenderView->Unbind();
}

void RenderViews::RenderImguiView(bool resized) {
    if (!resized) {
        ImGui::GetWindowDrawList()->AddImage(
            (void *)(uint64_t)m_RenderView->GetTextures().at(0)->GetID(),
            ImVec2(m_RenderViewportPos.x, m_RenderViewportPos.y),
            ImVec2(m_RenderViewportPos.x + m_RenderViewportSize.x, m_RenderViewportPos.y + m_RenderViewportSize.y),
            ImVec2(0, 1),
            ImVec2(1, 0));
    }
    ImGui::SetNextItemWidth(100);
    ImGui::Combo("##CurrentView", (int *)&m_CurrentView, "Render\0Normal\0Albedo \0Specular\0\0");
    auto e = GlobalEntitySelection::GetSelection();
    if (e && e.HasComponent<TransformComponent>()) {
        auto &c = m_CamController.GetCamera().GetComponent<CameraComponent>();
        auto &ct = m_CamController.GetCamera().GetComponent<TransformComponent>();
        auto &t = e.GetComponent<TransformComponent>();
        ImGuizmo::SetRect(m_RenderViewportPos.x, m_RenderViewportPos.y, m_RenderViewportSize.x, m_RenderViewportSize.y);
        ImGuizmo::SetDrawlist();
        glm::mat4 model = t.GetModelMatrix();
        glm::mat4 delta;
        float snap = (Rush::Input::KeyDown(RUSH_KEY_LEFT_SHIFT) || Rush::Input::KeyDown(RUSH_KEY_RIGHT_SHIFT)) ? 0.1f : 0.0f;
        if (snap != 0.0f)
            RUSH_LOG_INFO("Tist");
        ImGuizmo::Manipulate(&glm::inverse(ct.GetModelMatrix())[0].x, &c.camera.GetProjection()[0].x, m_GizmoOp, m_GizmoMode, &model[0].x, &delta[0].x, &snap);
        m_UsingGizmo = ImGuizmo::IsOver();
        if (ImGuizmo::IsUsing()) {
            glm::quat orient;
            glm::vec3 trans, scale, skew;
            glm::vec4 persp;
            glm::decompose(delta, scale, orient, trans, skew, persp);
            switch (m_GizmoOp) {
                case ImGuizmo::ROTATE:
                    orient.w = -orient.w;
                    t.Rotate(orient);
                    break;
                case ImGuizmo::TRANSLATE:
                    t.Translate(trans);
                    break;
                case ImGuizmo::SCALE:
                    t.Scale(scale);
                    break;
            }
        }
    }
}

void RenderViews::DoObjectPick(Rush::Scene &scene) {
    using namespace Rush;
    m_ObjectPick = false;
    auto [x, y] = Rush::Input::MousePos();
    auto p = m_SelectionBuffer->GetTextures().at(0)->GetProperties();
    glm::vec<2, int> texClickPos(x - m_RenderViewportPos.x, p.height - (y - m_RenderViewportPos.y));
    if (texClickPos.x < 0 || texClickPos.x >= p.width ||
        texClickPos.y < 0 || texClickPos.y >= p.height) {
        return;
    }

    m_SelectionBuffer->Bind();
    glm::vec4 color = glm::vec4(1.0f);
    color.r = ((float)(static_cast<std::underlying_type_t<entt::entity>>(entt::null) & 0x0000FF)) / 255.0f;
    color.g = ((float)((static_cast<std::underlying_type_t<entt::entity>>(entt::null) >> 8) & 0x0000FF)) / 255.0f;
    color.b = ((float)((static_cast<std::underlying_type_t<entt::entity>>(entt::null) >> 16) & 0x0000FF)) / 255.0f;
    color.a = ((float)((static_cast<std::underlying_type_t<entt::entity>>(entt::null) >> 24) & 0x0000FF)) / 255.0f;
    Renderer::GetAPI()->SetClearColor(color);
    Renderer::GetAPI()->Clear();
    auto &cam = m_CamController.GetCamera().GetComponent<CameraComponent>();
    auto &camTrans = m_CamController.GetCamera().GetComponent<TransformComponent>();
    glm::mat4 view = camTrans.GetModelMatrix();

    Renderer::BeginScene(cam.camera, view);

    auto reg = scene.GetRegistry();
    for (auto &e : reg->group<TransformComponent>(entt::get_t<MeshRendererComponent>())) {
        color.r = ((float)(entt::to_integral(e) & 0x0000FF)) / 255.0f;
        color.g = ((float)((entt::to_integral(e) >> 8) & 0x0000FF)) / 255.0f;
        color.b = ((float)((entt::to_integral(e) >> 16) & 0x0000FF)) / 255.0f;
        color.a = ((float)((entt::to_integral(e) >> 24) & 0x0000FF)) / 255.0f;
        m_SelectionShader->SetUniform("u_ID.color", ShaderData::FLOAT4, glm::value_ptr(color));
        auto [transform, mesh] = reg->get<TransformComponent, MeshRendererComponent>(e);
        glm::mat4 model = transform.GetModelMatrix();
        for (int i = 0; i < mesh.mesh->submeshes.size(); i++) {
            Renderer::Submit(m_SelectionShader, mesh.mesh->submeshes[i].vertices, model);
        }
    }

    for (auto e : reg->view<LightComponent>()) {
        color.r = ((float)(entt::to_integral(e) & 0x0000FF)) / 255.0f;
        color.g = ((float)((entt::to_integral(e) >> 8) & 0x0000FF)) / 255.0f;
        color.b = ((float)((entt::to_integral(e) >> 16) & 0x0000FF)) / 255.0f;
        color.a = ((float)((entt::to_integral(e) >> 24) & 0x0000FF)) / 255.0f;
        m_SelectionShader->SetUniform("u_ID.color", ShaderData::FLOAT4, glm::value_ptr(color));
        auto [l, t] = reg->get<LightComponent, TransformComponent>(e);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(t.GetModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
        model = glm::scale(model, glm::vec3(0.2f));
        Renderer::RenderCube(m_SelectionShader, model); // Fake until model specific render state is supported
    }

    for (auto e : reg->view<CameraComponent>()) {
        if (e == m_CamController.GetCamera()) continue;
        color.r = ((float)(entt::to_integral(e) & 0x0000FF)) / 255.0f;
        color.g = ((float)((entt::to_integral(e) >> 8) & 0x0000FF)) / 255.0f;
        color.b = ((float)((entt::to_integral(e) >> 16) & 0x0000FF)) / 255.0f;
        color.a = ((float)((entt::to_integral(e) >> 24) & 0x0000FF)) / 255.0f;
        m_SelectionShader->SetUniform("u_ID.color", ShaderData::FLOAT4, glm::value_ptr(color));
        auto [c, t] = reg->get<CameraComponent, TransformComponent>(e);
        glm::mat4 model = t.GetModelMatrix();
        for (int i = 0; i < m_CameraMesh->submeshes.size(); i++) {
            Renderer::Submit(m_SelectionShader, m_CameraMesh->submeshes[i].vertices, model);
        }
    }
    Renderer::EndScene();
    m_SelectionBuffer->Unbind();
    int size = p.width * p.height * p.bpp / 8 * p.channels;
    unsigned char *buffer = new unsigned char[size];

    m_SelectionBuffer->GetTextures().at(0)->GetTextureData(buffer, size);
    unsigned char r, g, b, a;

    int index = (texClickPos.x + p.width * texClickPos.y) * p.bpp / 8 * p.channels;
    r = buffer[index];
    g = buffer[index + 1];
    b = buffer[index + 2];
    a = buffer[index + 3];
    delete[] buffer;
    uint32_t id = (r) | (g << 8) | (b << 16) | (a << 24);
    entt::entity picked = (entt::entity)id;

    if (scene.GetRegistry()->valid(picked))
        GlobalEntitySelection::SetSelection({ reg, picked });
    else
        GlobalEntitySelection::ClearSelection();
}

bool RenderViews::MouseClickHandle(Rush::MouseReleasedEvent &e) {
    if (e.keycode != RUSH_MOUSE_BUTTON_LEFT || m_UsingGizmo || !m_Focused) return false;

    m_ObjectPick = true;
    return true;
}

bool RenderViews::KeyPressHandle(Rush::KeyboardPressEvent &e) {
    if (e.modifiers != RUSH_MOD_CONTROL) return false;

    switch (e.keycode) {
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

bool RenderViews::KeyReleaseHandle(Rush::KeyboardReleaseEvent &e) {
    return false;
}
