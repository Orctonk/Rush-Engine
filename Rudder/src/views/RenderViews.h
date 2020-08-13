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
    MeshInstance m_CameraMesh;
    Rush::Shared<Rush::Shader> m_LightBoxShader;
    Rush::Shared<Rush::Shader> m_SelectionShader;
    Rush::Shared<Rush::Shader> m_RenderViewShaders[RENDERVIEW_COUNT];
    Rush::Unique<Rush::Framebuffer> m_RenderViews[RenderView::RENDERVIEW_COUNT];
    Rush::Unique<Rush::Framebuffer> m_SelectionBuffer;
    CameraController m_CamController;
    glm::vec2 m_RenderViewportPos;
    glm::vec2 m_RenderViewportSize;
    bool m_ObjectPick;
    bool m_UsingGizmo;
    ImGuizmo::OPERATION m_GizmoOp;
    ImGuizmo::MODE m_GizmoMode;

    void FillRenderView(Rush::Scene &scene);
    void PopulateView(Rush::Scene &scene, RenderView type);
    void RenderImguiView(const char *name, RenderView type, bool resized);
    void DoObjectPick(Rush::Scene &scene);
    bool MouseClickHandle(Rush::MouseReleasedEvent &e);
    bool KeyPressHandle(Rush::KeyboardPressEvent &e);
    bool KeyReleaseHandle(Rush::KeyboardReleaseEvent &e);

public:
    bool enabledViews[RenderView::RENDERVIEW_COUNT];

    RenderViews();
    ~RenderViews();

    void Init(Rush::Entity cameraEntity);
    void OnUpdate(Rush::Scene &scene);
    void OnEvent(Rush::Event &e);
    void OnImguiRender();
};

#endif // __EDITORRENDERVIEWS_H__
