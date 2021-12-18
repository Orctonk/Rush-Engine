#include "TransformComponent.h"
#include "Rushpch.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

void TransformComponent::RecalcTransform() {
    m_Model = glm::toMat4(m_Rotation);
    m_Model = glm::translate(glm::mat4(1.0f), m_Translation) * m_Model;
    m_Model = glm::scale(m_Model, m_Scale);
    m_Dirty = false;
}

TransformComponent::TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
    : m_Translation(translation), m_Rotation(rotation), m_Scale(scale), m_Dirty(false) {
    RecalcTransform();
}

glm::mat4 TransformComponent::GetModelMatrix() {
    if (m_Dirty)
        RecalcTransform();
    return m_Model;
}

void TransformComponent::SetTranslation(const glm::vec3 &translation) {
    m_Dirty = true;
    m_Translation = translation;
}
void TransformComponent::SetRotation(const glm::quat &rotation) {
    m_Dirty = true;
    m_Rotation = rotation;
}
void TransformComponent::SetScale(const glm::vec3 &scale) {
    m_Dirty = true;
    m_Scale = scale;
}

void TransformComponent::Translate(const glm::vec3 &translation) {
    m_Dirty = true;
    m_Translation += translation;
}

void TransformComponent::Scale(const glm::vec3 &scale) {
    m_Dirty = true;
    m_Scale *= scale;
}

void TransformComponent::Rotate(const glm::quat &rotation) {
    m_Dirty = true;
    m_Rotation = glm::normalize(rotation * m_Rotation);
}