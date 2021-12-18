#include "LightComponent.h"

PackedLight LightComponent::Pack(const TransformComponent &t) const {
    PackedLight packed;
    glm::vec3 trans = t.GetTranslation();
    glm::quat rot = t.GetRotation();
    if (type != LightType::DIRECTIONAL) {
        packed.position_cutoff = glm::vec4(trans, glm::cos(glm::radians(cutoff)));
        if (type == LightType::POINT)
            packed.direction_cutoffOuter = glm::vec4(0.0f, 0.0f, 0.0f, glm::cos(glm::radians(outerCutoff)));
        else {
            packed.direction_cutoffOuter = glm::rotate(rot, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
            packed.direction_cutoffOuter.w = glm::cos(glm::radians(outerCutoff));
        }
    } else {
        packed.position_cutoff = glm::vec4(trans, cutoff);
        packed.direction_cutoffOuter = glm::rotate(rot, glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
        packed.direction_cutoffOuter.w = outerCutoff;
    }
    packed.ambient_constant = glm::vec4(ambient, constant);
    packed.diffuse_linear = glm::vec4(diffuse, linear);
    packed.specular_quadratic = glm::vec4(specular, quadratic);
    return packed;
}