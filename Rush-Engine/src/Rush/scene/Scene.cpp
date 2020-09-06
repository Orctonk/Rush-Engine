#include "Rushpch.h"
#include "Scene.h"

#include "Rush/core/Time.h"
#include "Rush/resources/AssetManager.h"
#include "Rush/graphics/Renderer.h"
#include "Components.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Rush {

Scene::Scene(){

}

Scene::~Scene(){
    
}

Entity Scene::NewEntity(std::string name){
    Entity e = {&m_SceneRegistry, m_SceneRegistry.create()};
    e.AddComponent<TransformComponent>();
    e.AddComponent<TagComponent>(name == "" ? "New entity" : name);
    return e;
}

void Scene::DeleteEntity(Entity e){
    m_SceneRegistry.remove_all(e.m_Entity);
    m_SceneRegistry.destroy(e.m_Entity);
}

void Scene::OnUpdate(){
    for(auto e : m_SceneRegistry.view<ParticleComponent>()){
        auto &pe = m_SceneRegistry.get<ParticleComponent>(e);
        pe.particleSystem.OnUpdate();
        if(pe.emissionRate <= 0.0f)
            continue;
        pe.timeSinceEmission += Time::Delta();
        for(;pe.timeSinceEmission > 1.0f/pe.emissionRate;pe.timeSinceEmission -= 1.0f/pe.emissionRate){
            pe.particleSystem.Emit(pe.emissionProperties);
        }
    }
}

void Scene::Render(){
    if(!m_SceneShader)
        m_SceneShader = AssetManager::GetShader("res/shaders/materialShader.glsl");
    if(!m_SkyboxShader)
        m_SkyboxShader = AssetManager::GetShader("res/shaders/skyboxShader.glsl");
    
    CameraComponent *mainCamera = nullptr;
    glm::mat4 view;

    for(auto e : m_SceneRegistry.view<CameraComponent>()){
        auto &c = m_SceneRegistry.get<CameraComponent>(e);
        if(c.main){
            auto &t = m_SceneRegistry.get<TransformComponent>(e);
            view = t.GetModelMatrix();
            
            mainCamera = &c;
        }
    }
    if(!mainCamera){
        Renderer::GetAPI()->SetClearColor(glm::vec4(0.0f,0.0f,0.0f,1.0f));
        Renderer::GetAPI()->Clear();
        return;
    }
    Renderer::GetAPI()->SetClearColor(mainCamera->clearColor);
    Renderer::GetAPI()->Clear();
    Renderer::BeginScene(mainCamera->camera,view);
    
    for(auto &e : m_SceneRegistry.group<TransformComponent>(entt::get_t<MeshRendererComponent>())){
        auto [transform, mesh] = m_SceneRegistry.get<TransformComponent,MeshRendererComponent>(e);
        glm::mat4 model = transform.GetModelMatrix();

        for(auto &sm : mesh.mesh->submeshes){
            sm.material->Bind();
            SetLightData(sm.material->materialShader);
            Renderer::Submit(sm.material,sm.vertices,model);
        }
    }

    for(auto &e : m_SceneRegistry.view<ParticleComponent>()){
        auto [transform, pe] = m_SceneRegistry.get<TransformComponent,ParticleComponent>(e);
        glm::mat4 model = transform.GetModelMatrix();
        pe.particleSystem.Render(view,model);
    }

    if(mainCamera->skybox != nullptr){
        mainCamera->skybox->Bind(0);
        Renderer::RenderCube(m_SkyboxShader,glm::mat4(1.0f));
    }
    Renderer::EndScene();
}

void Scene::SetLightData(Shared<Shader> shader){
    glm::vec4 plv;
    int lightCount = 0;
    for(auto e : m_SceneRegistry.view<LightComponent>()){
        auto &l = m_SceneRegistry.get<LightComponent>(e);
        if(l.type == LightType::DIRECTIONAL){
            auto &t = m_SceneRegistry.get<TransformComponent>(e);
            glm::vec3 trans = t.GetTranslation();
            glm::quat rot = t.GetRotation();
            plv = glm::vec4(trans,l.cutOff);
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].position_cutoff",ShaderData::FLOAT4,glm::value_ptr(plv));
            plv = glm::rotate(rot,glm::vec4(0.0f,0.0f,1.0f,0.0f));
            plv.w = l.outerCutOff;
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].direction_cutoffOuter",ShaderData::FLOAT4,glm::value_ptr(plv));
            plv = glm::vec4(l.ambient,l.constant);
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].ambient_constant",ShaderData::FLOAT4,glm::value_ptr(plv));
            plv = glm::vec4(l.diffuse,l.linear);
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].diffuse_linear",ShaderData::FLOAT4,glm::value_ptr(plv));
            plv = glm::vec4(l.specular,l.quadratic);
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].specular_quadratic",ShaderData::FLOAT4,glm::value_ptr(plv));
            lightCount++;
        }
    }
    shader->SetUniform("u_DLightCount",lightCount);
    for(auto e : m_SceneRegistry.view<LightComponent>()){
        auto &l = m_SceneRegistry.get<LightComponent>(e);
        if(l.type != LightType::DIRECTIONAL){
            auto &t = m_SceneRegistry.get<TransformComponent>(e);
            glm::vec3 trans = t.GetTranslation();
            glm::quat rot = t.GetRotation();
            plv = glm::vec4(trans,glm::cos(glm::radians(l.cutOff)));
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].position_cutoff",ShaderData::FLOAT4,glm::value_ptr(plv));
            if(l.type == LightType::POINT)
                plv = glm::vec4(0.0f,0.0f,0.0f,glm::cos(glm::radians(l.outerCutOff)));
            else {
                plv = glm::rotate(rot, glm::vec4(1.0f,0.0f,0.0f,0.0f));
                plv.w = glm::cos(glm::radians(l.outerCutOff));
            }
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].direction_cutoffOuter",ShaderData::FLOAT4,glm::value_ptr(plv));
            plv = glm::vec4(l.ambient,l.constant);
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].ambient_constant",ShaderData::FLOAT4,glm::value_ptr(plv));
            plv = glm::vec4(l.diffuse,l.linear);
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].diffuse_linear",ShaderData::FLOAT4,glm::value_ptr(plv));
            plv = glm::vec4(l.specular,l.quadratic);
            shader->SetUniform("u_Lights[" + std::to_string(lightCount) + "].specular_quadratic",ShaderData::FLOAT4,glm::value_ptr(plv));
            lightCount++;
        }
    }
    shader->SetUniform("u_LightCount",lightCount);
}

}