#include "Rushpch.h"
#include "ParticleSystem.h"

#include "Rush/resources/AssetManager.h"
#include "Rush/core/Random.h"
#include "Rush/core/Time.h"

#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/orthonormalize.hpp>

namespace Rush {

ParticleSystem::ParticleSystem(uint32_t poolSize){
    m_PoolIndex = 0;
    m_ParticlePool.resize(poolSize);
}

ParticleSystem::~ParticleSystem(){
    
}

void ParticleSystem::Emit(const ParticleProperties &particleProps){
    Particle &p = m_ParticlePool[m_PoolIndex];
    p.active = true;
    p.position = particleProps.position + GenRandomScaleVec() * particleProps.positionVariance;
    p.velocity = particleProps.velocity + GenRandomScaleVec() * particleProps.velocityVariance;
    p.color0 = particleProps.colorBegin;
    p.color1 = particleProps.colorEnd;
    p.rotation = particleProps.rotation + Random::GetFloat(-0.5f,0.5f) * particleProps.rotationVariance;
    p.angularVelocity = particleProps.angularVelocity + Random::GetFloat(-0.5f,0.5f) * particleProps.AVVariance;
    p.size0 = particleProps.sizeBegin + Random::GetFloat(-0.5f,0.5f) * particleProps.sizeVariance;
    p.size1 = particleProps.sizeEnd + Random::GetFloat(-0.5f,0.5f) * particleProps.sizeVariance;
    p.lifetime = particleProps.lifetime;
    p.lifeRemaining = p.lifetime;
    m_PoolIndex = (m_PoolIndex + 1) % m_ParticlePool.capacity(); 
}

void ParticleSystem::OnUpdate(){
    for(auto &p : m_ParticlePool){
        if(!p.active)
            continue;
        if(p.lifeRemaining <= 0.0f){
            p.active = false;
            continue;
        }

        float ts = Time::Delta();
        p.lifeRemaining -= ts;
        p.position += p.velocity * ts;
        p.rotation += p.angularVelocity * ts;
    }
}

void ParticleSystem::Render(const glm::mat4 &cameraTransform, const glm::mat4 &systemTransform){
    if(!m_PVA){
        m_PVA = VertexArray::Create();
        float data[] = { 
            -0.5f, 0.5f, 0.0f,
            -0.5f,-0.5f, 0.0f,
             0.5f,-0.5f, 0.0f,
             0.5f, 0.5f, 0.0f  
        };
        Shared<VertexBuffer> vb = VertexBuffer::Create(data,sizeof(data));
        vb->SetInterleavedLayout({
            ShaderData::FLOAT3
        });
        
        m_InstanceBuffer = VertexBuffer::Create(1000*(sizeof(glm::mat4)+sizeof(glm::vec4)));
        m_InstanceBuffer->SetInterleavedLayout({
            ShaderData::FLOAT4,
            ShaderData::MAT4
        });
        m_InstanceBuffer->SetInstanced(true);
        m_PVA->AddVertexBuffer(vb);
        m_PVA->AddVertexBuffer(m_InstanceBuffer);

        uint32_t indices[] = {
            0,1,2,
            2,3,0
        };
        auto ib = IndexBuffer::Create(indices,6);

        m_PVA->SetIndexBuffer(ib);
    }
    struct ParticleVertex {
        glm::vec4 color;
        glm::mat4 instanceMatrix;
    };
    ParticleVertex instanceData[1000];
    uint32_t count = 0;
    glm::mat4 cameraRotation = glm::orthonormalize(glm::mat3(cameraTransform));
    for(auto &p: m_ParticlePool){
        if(!p.active)
            continue;
        float lifePercent = p.lifeRemaining / p.lifetime;
        instanceData[count].color = glm::lerp(p.color1,p.color0,lifePercent);
        float size = glm::lerp(p.size1,p.size0,lifePercent);
        glm::mat4 transform = 
            glm::scale(
                glm::rotate(
                    glm::translate(glm::mat4(1.0f),p.position),
                    glm::radians(p.rotation),
                    glm::vec3(0.0f,0.0f,1.0f)
                ) * cameraRotation,
            {size,size,1.0f}
        );
        instanceData[count].instanceMatrix = transform;
        count++;
    }
    m_InstanceBuffer->BufferData(instanceData,count * sizeof(ParticleVertex));
    m_PVA->SetInstanceCount(count);
    Renderer::Submit(AssetManager::GetShader("res/shaders/particleShader.glsl"),m_PVA,systemTransform);

}

glm::vec3 ParticleSystem::GenRandomScaleVec(){
    return glm::vec3(Random::GetFloat(-0.5f,0.5f),Random::GetFloat(-0.5f,0.5f),Random::GetFloat(-0.5f,0.5f));
}

}