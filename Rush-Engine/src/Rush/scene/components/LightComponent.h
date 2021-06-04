#ifndef __LIGHTCOMPONENT_H__
#define __LIGHTCOMPONENT_H__

#include "Rush/core/Core.h"
#include "TransformComponent.h"

#include <string>
#include <glm/glm.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/string.hpp>
#include <entt/core/attribute.h>

enum class ENTT_API LightType {
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

struct PackedLight {
    glm::vec4 position_cutoff;
    glm::vec4 direction_cutoffOuter;
    glm::vec4 ambient_constant;
    glm::vec4 diffuse_linear;
    glm::vec4 specular_quadratic;
};

struct ENTT_API LightComponent {
    LightType type      {LightType::POINT};
    bool manualAtten    {false};
    float range         {7.0f};
    float constant      {1.0f};
    float linear        {0.7f};
    float quadratic     {1.8f};

    float cutOff        {35.0f};
    float outerCutOff   {45.0f};

    glm::vec3 ambient   {0.1f,0.1f,0.1f};
    glm::vec3 diffuse   {1.0f,1.0f,1.0f};
    glm::vec3 specular  {1.0f,1.0f,1.0f};

    PackedLight Pack(const TransformComponent &t) const;

    template<typename Archive>
    void save(Archive &archive) const{
        std::string typeStr;
        if(type == LightType::POINT)
            typeStr = "point";
        else if(type == LightType::DIRECTIONAL)
            typeStr = "directional";
        else
            typeStr = "spot";
        archive(
            cereal::make_nvp("type",typeStr),
            cereal::make_nvp("range",range),
            cereal::make_nvp("manualAttenuation",manualAtten),
            cereal::make_nvp("constant",constant),
            cereal::make_nvp("linear",linear),
            cereal::make_nvp("quadratic",quadratic),
            cereal::make_nvp("cutoff",cutOff),
            cereal::make_nvp("outerCutoff",outerCutOff),
            cereal::make_nvp("ambientColor",std::array{
                cereal::make_nvp("r",ambient.r),
                cereal::make_nvp("g",ambient.g),
                cereal::make_nvp("b",ambient.b)
            }),
            cereal::make_nvp("diffuseColor",std::array{
                cereal::make_nvp("r",diffuse.r),
                cereal::make_nvp("g",diffuse.g),
                cereal::make_nvp("b",diffuse.b)
            }),
            cereal::make_nvp("specularColor",std::array{
                cereal::make_nvp("r",specular.r),
                cereal::make_nvp("g",specular.g),
                cereal::make_nvp("b",specular.b)
            })
        );
    }

    template<typename Archive>
    void load(Archive &archive){
        std::string typeStr;
        
        archive(
            cereal::make_nvp("type",typeStr),
            cereal::make_nvp("range",range),
            cereal::make_nvp("manualAttenuation",manualAtten),
            cereal::make_nvp("constant",constant),
            cereal::make_nvp("linear",linear),
            cereal::make_nvp("quadratic",quadratic),
            cereal::make_nvp("cutoff",cutOff),
            cereal::make_nvp("outerCutoff",outerCutOff),
            cereal::make_nvp("ambientColor",std::array{
                cereal::make_nvp("r",ambient.r),
                cereal::make_nvp("g",ambient.g),
                cereal::make_nvp("b",ambient.b)
            }),
            cereal::make_nvp("diffuseColor",std::array{
                cereal::make_nvp("r",diffuse.r),
                cereal::make_nvp("g",diffuse.g),
                cereal::make_nvp("b",diffuse.b)
            }),
            cereal::make_nvp("specularColor",std::array{
                cereal::make_nvp("r",specular.r),
                cereal::make_nvp("g",specular.g),
                cereal::make_nvp("b",specular.b)
            })
        );

        if(typeStr == "point")
            type = LightType::POINT;
        else if(typeStr == "directional")
            type = LightType::DIRECTIONAL;
        else
            type = LightType::SPOTLIGHT;
    }
};

#endif // __LIGHTCOMPONENT_H__
