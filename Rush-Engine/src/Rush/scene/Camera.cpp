#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Rush {

void Camera::recalcCamera(){
    m_Front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
    m_Front.y = glm::sin(glm::radians(m_Pitch));
    m_Front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

    m_Front = glm::normalize(m_Front);
    m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0,1,0)));
    glm::vec3 up = glm::normalize(glm::cross(m_Right,m_Front));

    m_View = glm::lookAt(m_Position,m_Position + m_Front,glm::vec3(0,1,0));
    m_VP = m_Projection * m_View;
}

Camera::Camera(ProjectionMode mode, float aspect) 
:   m_Yaw(0), m_Pitch(0), m_Roll(0), m_FOV(45), m_Position(glm::vec3(0,0,1)), m_Front(), m_Right() {
    m_Projection = glm::mat4(1.0f);
    m_View = glm::mat4(1.0f);
    recalcCamera();
    SetProjection(mode,aspect);
}

Camera::~Camera() { }

void Camera::SetPosition(glm::vec3 pos){
    m_Position = pos;
    recalcCamera();
}

void Camera::SetRotation(float yaw, float pitch, float roll){
    m_Yaw = yaw;
    m_Pitch = pitch;
    m_Roll = roll;
    recalcCamera();
}

void Camera::SetProjection(ProjectionMode mode, float aspect) {
    if(mode == ProjectionMode::PERSPECTIVE){
        m_Projection = glm::perspective(glm::radians(m_FOV),aspect,0.1f,100.0f);
    } else {
        m_Projection = glm::ortho(-1.0f,1.0f,-1/aspect,1/aspect,0.1f,100.0f);
    }
    m_VP = m_Projection * m_View;
}

void Camera::Rotate(float yaw, float pitch, float roll) {
    m_Yaw += yaw;
    m_Pitch += pitch;
    m_Roll += roll;    
    recalcCamera();
}

void Camera::Translate(glm::vec3 offset){
    m_Position += offset;
    recalcCamera();
}

}