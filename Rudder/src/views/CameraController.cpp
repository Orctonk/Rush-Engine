#include "CameraController.h"

CameraController::CameraController(Rush::Camera camera)
:   m_Camera(camera), m_CurDragMode(MouseDragMode::NONE) { }

CameraController::~CameraController() { }

void CameraController::OnUpdate() { }
void CameraController::OnEvent(Rush::Event &e){
    e.Dispatch<Rush::MousePressedEvent>(RUSH_BIND_FN(CameraController::MousePressHandle));
    e.Dispatch<Rush::MouseReleasedEvent>(RUSH_BIND_FN(CameraController::MouseReleaseHandle));
    e.Dispatch<Rush::MouseMoveEvent>(RUSH_BIND_FN(CameraController::MouseMoveHandle));
    e.Dispatch<Rush::MouseScrollEvent>(RUSH_BIND_FN(CameraController::MouseScrollHandle));
    e.Dispatch<Rush::WindowResizeEvent>(RUSH_BIND_FN(CameraController::WindowResizeHandle));
}

bool CameraController::MousePressHandle(Rush::MousePressedEvent &e){
    auto [x,y] = Rush::Input::MousePos();
    m_DragLastX = x;
    m_DragLastY = y;

    switch(e.keycode){
        case RUSH_MOUSE_BUTTON_MIDDLE: m_CurDragMode = MouseDragMode::ROTATE; break;
        case RUSH_MOUSE_BUTTON_RIGHT: m_CurDragMode = MouseDragMode::PAN; break;
    }
    return false;
}

bool CameraController::MouseReleaseHandle(Rush::MouseReleasedEvent &e){
    switch(e.keycode){
        case RUSH_MOUSE_BUTTON_MIDDLE: 
            m_CurDragMode = m_CurDragMode == MouseDragMode::ROTATE ? MouseDragMode::NONE : m_CurDragMode; 
            break;
        case RUSH_MOUSE_BUTTON_RIGHT: 
            m_CurDragMode = m_CurDragMode == MouseDragMode::PAN ? MouseDragMode::NONE : m_CurDragMode; 
            break;
        default:
            break;
    }
    return false;
}

bool CameraController::MouseMoveHandle(Rush::MouseMoveEvent &e){
    int dx = e.x - m_DragLastX;
    int dy = e.y - m_DragLastY;
    m_DragLastX = e.x;
    m_DragLastY = e.y;
    switch(m_CurDragMode){
        case MouseDragMode::ROTATE:
            m_Camera.Rotate(dx / 2.0f, -dy / 2.0f, 0);
            m_Camera.Translate(
                m_Camera.GetRight() * sinf(-dx/180.0f * 3.14) * 2.0f +
                m_Camera.GetFront() * (1-cosf(-dx/180.0f * 3.14)) * 2.0f +
                glm::vec3(0.0f,1.0f,0.0f) * sinf(dy/180.0f * 3.14) * 2.0f +
                m_Camera.GetFront() * (1-cosf(dy/180.0f * 3.14)) * 2.0f
            );
            break;
        case MouseDragMode::PAN:
            m_Camera.Translate(
                m_Camera.GetRight() * (-dx / 200.0f) +
                glm::vec3(0.0f, (dy / 200.0f), 0.0f) );
            break;
    }
    return false;
}

bool CameraController::MouseScrollHandle(Rush::MouseScrollEvent &e){
    m_Camera.Translate(m_Camera.GetFront() * (e.delta /10.0f));
    return false;
}

bool CameraController::WindowResizeHandle(Rush::WindowResizeEvent &e){
    m_Camera.SetProjection(Rush::ProjectionMode::PERSPECTIVE, ((float)e.width)/e.height);
    return false;
}