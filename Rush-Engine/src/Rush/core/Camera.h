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
    glm::mat4 m_VP;
    glm::mat4 m_View;
    glm::mat4 m_Projection;
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Right;

    float m_Pitch;
    float m_Yaw;
    float m_Roll;
    float m_FOV;

    void recalcCamera();
public:
    Camera(ProjectionMode mode);
    ~Camera();

    void SetPosition(glm::vec3 position);
    void SetRotation(float yaw, float pitch, float roll);
    void SetProjection(ProjectionMode mode);
    void Rotate(float yaw, float pitch, float roll);
    void Translate(glm::vec3 offset);

    glm::mat4 GetVPMatrix() { return m_VP; }
    glm::mat4 GetProjection() { return m_Projection; }
    glm::mat4 GetView() { return m_View; }
};

}

#endif // __CAMERA_H__
