#include "CameraController.h"

#include <glm/gtx/euler_angles.hpp>

CameraController::CameraController()
:   m_CurDragMode(MouseDragMode::NONE), m_YPR(0.0f,0.0f,0.0f) { }

CameraController::~CameraController() { }

void CameraController::SetControlledCamera(Rush::Entity camera){
    m_Camera = camera;
    TransformComponent &tc = camera.GetComponent<TransformComponent>();
    auto rot = tc.GetRotation();
    m_YPR.y = glm::degrees(glm::yaw(rot));
    m_YPR.x = glm::degrees(glm::pitch(rot));
    m_YPR.z = glm::degrees(glm::roll(rot));
}

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

    auto &t = m_Camera.GetComponent<TransformComponent>();
    glm::quat rot = t.GetRotation();

    glm::vec3 right = glm::rotate(rot, glm::vec4(1.0f,0.0f,0.0f,0.0f));
    glm::vec3 front = glm::rotate(rot, glm::vec4(0.0f,0.0f,-1.0f,0.0f));
    switch(m_CurDragMode){
        case MouseDragMode::ROTATE:
            m_YPR += glm::vec3(-dy / 2.0f,-dx / 2.0f,0.0f);
            t.SetRotation(glm::radians(m_YPR));
            t.Translate(glm::vec3(
                right * sinf(-dx/180.0f * 3.14) * 2.0f +
                front * (1-cosf(-dx/180.0f * 3.14)) * 2.0f +
                glm::vec3(0.0f,1.0f,0.0f) * sinf(dy/180.0f * 3.14) * 2.0f +
                front * (1-cosf(-dy/180.0f * 3.14)) * 2.0f
            ));
            break;
        case MouseDragMode::PAN:
            t.Translate(glm::vec3(
                right * (-dx / 200.0f) +
                glm::cross(right,front) * (dy / 200.0f)));
            break;
    }
    
    return false;
}

bool CameraController::MouseScrollHandle(Rush::MouseScrollEvent &e){
    auto &t = m_Camera.GetComponent<TransformComponent>();
    glm::quat rot = t.GetRotation();
    glm::vec3 front = glm::rotate(rot, glm::vec4(0.0f,0.0f,-1.0f,0.0f));
    t.Translate(glm::vec3(front * (e.delta /10.0f)));
    return false;
}

bool CameraController::WindowResizeHandle(Rush::WindowResizeEvent &e){
    if (e.height == 0)  // Window is most likely minimized
        return false;
    auto &c = m_Camera.GetComponent<CameraComponent>();
    c.camera.SetPerspective(((float)e.width)/e.height,45.0f);
    return false;
}