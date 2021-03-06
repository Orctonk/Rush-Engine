#ifndef __EDITORRENDERVIEWS_H__
#define __EDITORRENDERVIEWS_H__

#include <Rush.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include "CameraController.h"

enum RenderView{
    RENDERVIEW_RENDER   = 0,
    RENDERVIEW_NORMALS  = 1,
    RENDERVIEW_ALBEDO   = 2,
    RENDERVIEW_SPECULAR = 3,
    RENDERVIEW_COUNT    = 4
};

class RenderViews {
private:
    Rush::Shared<Rush::RootMesh> m_CameraMesh;
    Rush::Shared<Rush::Texture> m_SpotlightTexture;
    Rush::Shared<Rush::Texture> m_DirlightTexture;
    Rush::Shared<Rush::Shader> m_SelectionShader;
    Rush::Shared<Rush::Shader> m_SkyboxShader;
    Rush::Shared<Rush::Shader> m_RenderViewShaders[RENDERVIEW_COUNT];
    Rush::Unique<Rush::Framebuffer> m_RenderView;
    Rush::Unique<Rush::Framebuffer> m_SelectionBuffer;
    CameraController m_CamController;
    glm::vec2 m_RenderViewportPos;
    glm::vec2 m_RenderViewportSize;
    bool m_ObjectPick;
    bool m_UsingGizmo;
    bool m_Focused;
    ImGuizmo::OPERATION m_GizmoOp;
    ImGuizmo::MODE m_GizmoMode;
    RenderView m_CurrentView;

    void FillRenderView(Rush::Scene &scene);
    void PopulateView(Rush::Scene &scene, RenderView type);
    void RenderImguiView(bool resized);
    void DoObjectPick(Rush::Scene &scene);
    bool MouseClickHandle(Rush::MouseReleasedEvent &e);
    bool KeyPressHandle(Rush::KeyboardPressEvent &e);
    bool KeyReleaseHandle(Rush::KeyboardReleaseEvent &e);

public:
    bool enabled;

    RenderViews();
    ~RenderViews();

    void Init(Rush::Entity cameraEntity);
    void OnUpdate(Rush::Scene &scene);
    void OnEvent(Rush::Event &e);
    void OnImguiRender();
};

#endif // __EDITORRENDERVIEWS_H__
