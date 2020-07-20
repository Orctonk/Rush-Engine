#ifndef __EDITORRENDERVIEWS_H__
#define __EDITORRENDERVIEWS_H__

#include <Rush.h>
#include <glm/glm.hpp>
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
    Rush::Shared<Rush::Shader> m_MaterialShader;
    Rush::Shared<Rush::Shader> m_LightBoxShader;
    Rush::Shared<Rush::Shader> m_RenderViewShaders[RENDERVIEW_COUNT];
    Rush::Unique<Rush::Framebuffer> m_GBuffer;
    Rush::Unique<Rush::Framebuffer> m_RenderViews[RenderView::RENDERVIEW_COUNT];
    CameraController m_CamController;
    glm::vec2 m_RenderViewportSize;

    void RenderGBuffer(Rush::Scene &scene, Rush::Camera &cam);
    void FillRenderView(Rush::Scene &scene, Rush::Camera &cam);
    void PopulateView(RenderView type);
    void RenderImguiView(const char *name, RenderView type, bool resized);

public:
    bool enabledViews[RenderView::RENDERVIEW_COUNT];

    RenderViews();
    ~RenderViews();

    void Init();
    void OnUpdate(Rush::Scene &scene);
    void OnEvent(Rush::Event &e);
    void OnImguiRender();
};

#endif // __EDITORRENDERVIEWS_H__
