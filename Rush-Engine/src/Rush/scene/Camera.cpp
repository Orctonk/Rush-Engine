#include "Camera.h"
#include "Rushpch.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Rush {

Camera::Camera(float aspect, float FOV, float near, float far) {
    RUSH_PROFILE_FUNCTION();
    SetPerspective(aspect, FOV);
}

Camera::Camera(float left, float right, float top, float bottom, float near, float far) {
    RUSH_PROFILE_FUNCTION();
    SetOrthographic(left, right, top, bottom, near, far);
}

Camera::~Camera() { }

void Camera::SetPerspective(float aspect, float FOV, float near, float far) {
    RUSH_PROFILE_FUNCTION();
    m_LeftOrAspect = aspect;
    m_RightOrFOV = FOV;
    m_Near = near;
    m_Far = far;
    m_Bottom = m_Top = 0.0f;
    m_Projection = glm::perspective(glm::radians(FOV), aspect, near, far);
}

void Camera::SetOrthographic(float left, float right, float top, float bottom, float near, float far) {
    RUSH_PROFILE_FUNCTION();
    m_LeftOrAspect = left;
    m_RightOrFOV = right;
    m_Top = top;
    m_Bottom = bottom;
    m_Near = near;
    m_Far = far;
    m_Projection = glm::ortho(left, right, bottom, top, near, far);
}
} // namespace Rush