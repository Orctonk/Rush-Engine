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

Scene::Scene() {
    m_LightUniformBuffer = UniformBuffer::Create(sizeof(LightBuffer), 1);
}

Scene::~Scene() {
    
}

Entity Scene::NewEntity(std::string name) {
    Entity e = { &m_SceneRegistry, m_SceneRegistry.create() };
    e.AddComponent<TransformComponent>();
    e.AddComponent<TagComponent>(name == "" ? "New entity" : name);
    return e;
}

void Scene::DeleteEntity(Entity e) {
    m_SceneRegistry.remove_all(e.m_Entity);
    m_SceneRegistry.destroy(e.m_Entity);
}

void Scene::OnUpdate() {
    for (auto e : m_SceneRegistry.view<ParticleComponent>()) {
        auto &pe = m_SceneRegistry.get<ParticleComponent>(e);
        pe.particleSystem.OnUpdate();
        if (pe.emissionRate <= 0.0f)
            continue;
        pe.timeSinceEmission += Time::Delta();
        for (;pe.timeSinceEmission > 1.0f / pe.emissionRate;pe.timeSinceEmission -= 1.0f / pe.emissionRate) {
            pe.particleSystem.Emit(pe.emissionProperties);
        }
    }
}

void Scene::Render() {
    if (!m_SceneShader)
        m_SceneShader = AssetManager::GetShader("res/shaders/materialShader.glsl");
    if (!m_SkyboxShader)
        m_SkyboxShader = AssetManager::GetShader("res/shaders/skyboxShader.glsl");
    
    CameraComponent *mainCamera = nullptr;
    glm::mat4 view;

    for (auto e : m_SceneRegistry.view<CameraComponent>()) {
        auto &c = m_SceneRegistry.get<CameraComponent>(e);
        if (c.main) {
            auto &t = m_SceneRegistry.get<TransformComponent>(e);
            view = t.GetModelMatrix();
            
            mainCamera = &c;
        }
    }
    if (!mainCamera) {
        Renderer::GetAPI()->SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        Renderer::GetAPI()->Clear();
        return;
    }
    Renderer::GetAPI()->SetClearColor(mainCamera->clearColor);
    Renderer::GetAPI()->Clear();
    Renderer::BeginScene(mainCamera->camera, view);
    
    for (auto &e : m_SceneRegistry.group<TransformComponent>(entt::get_t<MeshRendererComponent>())) {
        auto [transform, mesh] = m_SceneRegistry.get<TransformComponent, MeshRendererComponent>(e);
        glm::mat4 model = transform.GetModelMatrix();

        for (auto &sm : mesh.mesh->submeshes) {
            sm.material->Bind();
            SetLightData(sm.material->materialShader);
            Renderer::Submit(sm.material, sm.vertices, model);
        }
    }

    for (auto &e : m_SceneRegistry.view<ParticleComponent>()) {
        auto [transform, pe] = m_SceneRegistry.get<TransformComponent, ParticleComponent>(e);
        glm::mat4 model = transform.GetModelMatrix();
        pe.particleSystem.Render(view, model);
    }

    if (mainCamera->skybox != nullptr) {
        mainCamera->skybox->Bind(3);
        Renderer::RenderCube(m_SkyboxShader, glm::mat4(1.0f));
    }
    Renderer::EndScene();
}

void Scene::StartPlay() {
}

void Scene::StopPlay() {
}

void Scene::SetLightData(Shared<Shader> shader) {
    m_LightBuffer.lightCount = 0;
    for (auto e : m_SceneRegistry.view<LightComponent>()) {
        auto &l = m_SceneRegistry.get<LightComponent>(e);
        if (l.type == LightType::DIRECTIONAL) {
            auto &t = m_SceneRegistry.get<TransformComponent>(e);
            m_LightBuffer.lights[m_LightBuffer.lightCount++] = l.Pack(t);
        }
    }
    m_LightBuffer.dirLightCount = m_LightBuffer.lightCount;
    for (auto e : m_SceneRegistry.view<LightComponent>()) {
        auto &l = m_SceneRegistry.get<LightComponent>(e);
        if (l.type != LightType::DIRECTIONAL) {
            auto &t = m_SceneRegistry.get<TransformComponent>(e);
            m_LightBuffer.lights[m_LightBuffer.lightCount++] = l.Pack(t);
        }
    }
    m_LightUniformBuffer->SetData(&m_LightBuffer, sizeof(LightBuffer));
    m_LightUniformBuffer->Bind(1);
}

}