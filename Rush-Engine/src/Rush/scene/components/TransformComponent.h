#ifndef __TRANSFORMCOMPONENT_H__
#define __TRANSFORMCOMPONENT_H__

#include "Rush/core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <entt/core/attribute.h>

struct ENTT_API TransformComponent {
private:
    glm::vec3 m_Translation;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
    glm::mat4 m_Model;
    bool m_Dirty;

    void RecalcTransform();

public:
    TransformComponent()
    : m_Translation(0.0f), m_Rotation(), m_Scale(1.0f), m_Model(1.0f), m_Dirty(true) {}

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

    template<typename Archive>
    void serialize(Archive &archive){
        archive(
            cereal::make_nvp("position",std::array{
                cereal::make_nvp("x", m_Translation.x),
                cereal::make_nvp("y", m_Translation.y),
                cereal::make_nvp("z", m_Translation.z)
            }),
            cereal::make_nvp("orientation",std::array{
                cereal::make_nvp("x", m_Rotation.x),
                cereal::make_nvp("y", m_Rotation.y),
                cereal::make_nvp("z", m_Rotation.z),
                cereal::make_nvp("w", m_Rotation.w)
            }),
            cereal::make_nvp("scale",std::array{
                cereal::make_nvp("x", m_Scale.x),
                cereal::make_nvp("y", m_Scale.y),
                cereal::make_nvp("z", m_Scale.z)
            })
        );
    }
};

#endif // __TRANSFORMCOMPONENT_H__
