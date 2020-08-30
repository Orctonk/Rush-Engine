#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "Camera.h"
#include "Rush/graphics/particles/ParticleSystem.h"
#include "Rush/graphics/Cubemap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

enum class RUSH_API LightType {
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

struct TransformComponent {
private:
    glm::vec3 m_Translation;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
    glm::mat4 m_Model;
    bool m_Dirty;

    void RecalcTransform(){
        m_Model = glm::toMat4(m_Rotation);
        m_Model = glm::translate(glm::mat4(1.0f),m_Translation) * m_Model;
        m_Model = glm::scale(m_Model,m_Scale);
        m_Dirty = false;
    }

public:
    TransformComponent()
    : m_Translation(0.0f), m_Rotation(), m_Scale(1.0f), m_Model(1.0f), m_Dirty(false) {}

    TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
    : m_Translation(translation),m_Rotation(rotation),m_Scale(scale),m_Dirty(false) {
        RecalcTransform();
    }

    glm::mat4 GetModelMatrix(){ 
        if(m_Dirty) RecalcTransform();
        return m_Model;
    }

    glm::vec3 GetTranslation() { return m_Translation; }
    glm::quat GetRotation() { return m_Rotation; }
    glm::vec3 GetScale() { return m_Scale; }

    void SetTranslation(const glm::vec3 &translation) { 
        m_Dirty = true; 
        m_Translation = translation; 
    }
    void SetRotation(const glm::quat &rotation) { 
        m_Dirty = true; 
        m_Rotation = rotation; 
    }
    void SetScale(const glm::vec3 &scale) { 
        m_Dirty = true; 
        m_Scale = scale; 
    }

    void Translate(const glm::vec3 &translation){
        m_Dirty = true;
        m_Translation += translation;
    }

    void Scale(const glm::vec3 &scale){
        m_Dirty = true;
        m_Scale *= scale;
    }

    void Rotate(const glm::quat &rotation){
        m_Dirty = true;
        m_Rotation = rotation * m_Rotation;
    }
};

struct CameraComponent {
    bool main;
    glm::vec4 clearColor;
    Rush::Shared<Rush::Cubemap> skybox;
    Rush::Camera camera;

    CameraComponent() 
        : main(false), clearColor(0.0f,0.0f,0.0f,1.0f), skybox(nullptr), camera(1024.0f/720.0f,45.0f) { }

    CameraComponent(bool main, Rush::Camera cam) 
        : main(main), clearColor(0.0f,0.0f,0.0f,1.0f), skybox(nullptr), camera(cam) { }
};

struct RUSH_API LightComponent {
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
};

struct RUSH_API ParticleEmitterComponent {
    Rush::ParticleSystem particleSystem;
    Rush::ParticleProperties emissionProperties;
    float emissionRate      {0.0f};
    float timeSinceEmission {0.0f};
};

#endif // __COMPONENTS_H__
