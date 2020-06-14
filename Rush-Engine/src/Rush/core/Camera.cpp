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

    m_View = glm::lookAt(m_Position,m_Position - m_Front,glm::vec3(0,1,0));
}

Camera::Camera(ProjectionMode mode) 
:   m_Yaw(0), m_Pitch(0), m_Roll(0), m_FOV(45), m_Position(glm::vec3(0,0,1)), m_Front(), m_Right() {
    m_Projection = glm::mat4(1.0f);
    m_View = glm::mat4(1.0f);
    recalcCamera();
    SetProjection(mode);
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

void Camera::SetProjection(ProjectionMode mode) {
    if(mode == ProjectionMode::PERSPECTIVE){
        m_Projection = glm::perspective(glm::radians(m_FOV),640.0f/480.0f,0.1f,100.0f);
    } else {
        m_Projection = glm::ortho(0.0f,640.0f,0.0f,480.0f);
    }
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