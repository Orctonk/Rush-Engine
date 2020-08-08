#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "Camera.h"
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
    glm::mat4 model;

    TransformComponent(){
        model = glm::mat4(1.0f);
    }

    TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale){
        model = glm::yawPitchRoll(glm::radians(rotation.y),glm::radians(rotation.x),glm::radians(rotation.z));
        model = glm::translate(glm::mat4(1.0f),translation) * model;
        model = glm::scale(model,scale);
    }

    glm::mat4 GetModelMatrix(){ 
        return model;
    }

    void Decompose(glm::vec3 &translation, glm::vec3 &rotation, glm::vec3 &scale){
        glm::quat orientation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(model,scale,orientation,translation,skew,perspective);
        rotation.x = glm::pitch(orientation);
        rotation.y = glm::yaw(orientation);
        rotation.z = glm::roll(orientation);
    }

    void Recompose(const glm::vec3 &translation, const glm::vec3 &rotation, const glm::vec3 &scale){
        model = glm::yawPitchRoll(glm::radians(rotation.y),glm::radians(rotation.x),glm::radians(rotation.z));
        model = glm::scale(model, scale);
        model = glm::translate(glm::mat4(1.0f),translation) * model;
    }

    void Translate(const glm::vec3 &translation){
        model = glm::translate(glm::mat4(1.0f),translation) * model;
    }

    void Scale(const glm::vec3 &scale){
        model = glm::scale(model,scale);
    }
};

struct CameraComponent {
    bool main;
    glm::vec4 clearColor;
    Rush::Camera camera;

    CameraComponent() 
        : main(false), clearColor(0.0f,0.0f,0.0f,1.0f), camera(1024.0f/720.0f,45.0f) { }

    CameraComponent(bool main, Rush::Camera cam) 
        : main(main), clearColor(0.0f,0.0f,0.0f,1.0f), camera(cam) { }
};

struct RUSH_API LightComponent {
    LightType type      {LightType::POINT};
    float constant      {1.0f};
    float linear        {0.7f};
    float quadratic     {1.8f};

    float cutOff        {35.0f};
    float outerCutOff   {45.0f};

    glm::vec3 ambient   {0.1f,0.1f,0.1f};
    glm::vec3 diffuse   {1.0f,1.0f,1.0f};
    glm::vec3 specular  {1.0f,1.0f,1.0f};
};

#endif // __COMPONENTS_H__
