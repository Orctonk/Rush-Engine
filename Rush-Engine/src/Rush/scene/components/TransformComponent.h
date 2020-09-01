#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

struct TransformComponent {
private:
    glm::vec3 m_Translation;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
    glm::mat4 m_Model;
    bool m_Dirty;

    void RecalcTransform();

public:
    TransformComponent()
    : m_Translation(0.0f), m_Rotation(), m_Scale(1.0f), m_Model(1.0f), m_Dirty(false) {}

    TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);

    glm::mat4 GetModelMatrix();
    glm::vec3 GetTranslation() { return m_Translation; }
    glm::quat GetRotation() { return m_Rotation; }
    glm::vec3 GetScale() { return m_Scale; }
    void SetTranslation(const glm::vec3 &translation);
    void SetRotation(const glm::quat &rotation);
    void SetScale(const glm::vec3 &scale);
    void Translate(const glm::vec3 &translation);
    void Scale(const glm::vec3 &scale);
    void Rotate(const glm::quat &rotation);
};

#endif // __TRANSFORMCOMPONENT_H__
