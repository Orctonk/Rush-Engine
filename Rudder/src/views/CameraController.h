#ifndef __EDITORCAMERACONTROLLER_H__
#define __EDITORCAMERACONTROLLER_H__

#include <Rush.h>

enum class MouseDragMode{
    NONE,
    PAN,
    ROTATE,
    ZOOM
};

class CameraController{
private:
    Rush::Camera m_Camera;
    int m_DragLastX, m_DragLastY;
    MouseDragMode m_CurDragMode;

    bool MousePressHandle(Rush::MousePressedEvent &e);
    bool MouseReleaseHandle(Rush::MouseReleasedEvent &e);
    bool MouseMoveHandle(Rush::MouseMoveEvent &e);
    bool MouseScrollHandle(Rush::MouseScrollEvent &e);
    bool WindowResizeHandle(Rush::WindowResizeEvent &e);
public:
    CameraController(Rush::Camera camera);
    ~CameraController();

    void OnUpdate();
    void OnEvent(Rush::Event &e);
    Rush::Camera &GetCamera() { return m_Camera; }
};

#endif // __EDITORCAMERACONTROLLER_H__
