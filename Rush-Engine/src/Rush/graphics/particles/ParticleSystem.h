#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "Rush/core/Core.h"
#include "Rush/graphics/Renderer.h"
#include "Rushpch.h"

#include <glm/glm.hpp>

namespace Rush {

struct RUSH_API ParticleProperties {
    glm::vec3 position; 
    glm::vec3 positionVariance  {0.0f,0.0f,0.0f};
    glm::vec3 velocity          {0.0f,0.0f,0.0f};
    glm::vec3 velocityVariance  {0.0f,0.0f,0.0f};
    glm::vec4 colorBegin        {1.0f,1.0f,1.0f,1.0f};
    glm::vec4 colorEnd          {1.0f,1.0f,1.0f,1.0f};
    float rotation              {0.0f};
    float rotationVariance      {0.0f};
    float angularVelocity       {0.0f};
    float AVVariance            {0.0f};
    float sizeBegin             {1.0f};
    float sizeEnd               {1.0f};
    float sizeVariance          {0.0f};
    float lifetime              {1.0f}; 
    float lifetimeVariance      {0.0f};
};

struct Particle{
    // Animated particle properties
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color0,color1;
    float rotation,angularVelocity;
    float size0,size1;

    // Particle lifetime properties
    float lifetime,lifeRemaining;
    bool active;
};

class RUSH_API ParticleSystem {
private:
    std::vector<Particle> m_ParticlePool;
    uint32_t m_PoolIndex;

    Shared<VertexArray> m_PVA = nullptr;
    Shared<VertexBuffer> m_InstanceBuffer;
    glm::vec3 GenRandomScaleVec();
public:
    ParticleSystem(uint32_t poolSize = 1000);
    ~ParticleSystem();

    void Emit(const ParticleProperties &particleProps);
    void OnUpdate();
    void Render(const glm::mat4 &cameraTransform, const glm::mat4 &systemTransform);
    std::vector<Particle> &GetParticlePool(){ return m_ParticlePool; }
};

}

#endif // __PARTICLESYSTEM_H__
