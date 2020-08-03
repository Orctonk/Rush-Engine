#ifndef __SCENEVIEW_H__
#define __SCENEVIEW_H__

#include <Rush.h>
#include <glm/glm.hpp>

class SceneView {
private:
    Rush::Unique<Rush::Framebuffer> m_SceneBuffer;
    glm::vec2 m_SceneViewSize;


public:
    bool enabled;

    SceneView();
    ~SceneView();

    void Init();
    void OnUpdate(Rush::Scene &scene);
    void OnImguiRender();
};

#endif // __SCENEVIEW_H__
