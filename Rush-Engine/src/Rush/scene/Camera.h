#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Rush/core/Core.h"

#include <glm/glm.hpp>

namespace Rush {

enum class ProjectionMode{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class RUSH_API Camera {
private:
    glm::mat4 m_Projection;
    float m_LeftOrAspect,m_RightOrFOV;
    float m_Near,m_Far;

    // Orthographic properties
    float m_Top,m_Bottom;
public:
    Camera(float aspect, float FOV, float near = 0.1f, float far = 100.0f);
    Camera(float left, float right, float top, float bottom, float near= 0.1f, float far = 100.0f);
    ~Camera();

    void SetPerspective(float aspect, float FOV, float near = 0.1f, float far = 100.0f);
    void SetOrthographic(float left, float right, float top, float bottom, float near = 0.1f, float far = 100.0f);

    float GetNear(){ return m_Near; }
    float GetFar(){ return m_Far; }

    // Orthographic camera specific
    float GetLeft(){ return m_LeftOrAspect; }
    float GetRight(){ return m_RightOrFOV; }
    float GetTop(){ return m_Top; }
    float GetBottom(){ return m_Bottom; }

    // Perspective camera specific
    float GetAspect(){ return m_LeftOrAspect; }
    float GetFOV(){ return m_RightOrFOV; }

    bool IsPerspective(){ return m_Top == m_Bottom; }

    glm::mat4 GetProjection() { return m_Projection; }
};

}

#endif // __CAMERA_H__
