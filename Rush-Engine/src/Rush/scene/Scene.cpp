#include "Scene.h"

#include "Rush/resources/AssetManager.h"
#include "Rush/resources/MeshInstance.h"
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

Entity Scene::NewEntity(){
    return {&m_SceneRegistry, m_SceneRegistry.create()};
}

void Scene::DeleteEntity(Entity e){
    m_SceneRegistry.remove_all(e.m_Entity);
    m_SceneRegistry.destroy(e.m_Entity);
}

void Scene::Render(){
    if(!m_SceneShader)
        m_SceneShader = AssetManager::GetShader("res/materialShader.glsl");

    Renderer::GetAPI()->Clear();
    
    Camera *mainCamera = nullptr;
    glm::mat4 view;

    for(auto e : m_SceneRegistry.view<CameraComponent>()){
        auto &c = m_SceneRegistry.get<CameraComponent>(e);
        if(c.main){
            auto &t  = m_SceneRegistry.get<TransformComponent>(e);
            view = glm::eulerAngleZYX(glm::radians(t.rotation.x),glm::radians(t.rotation.y),glm::radians(t.rotation.z));
            view = glm::translate(glm::mat4(1.0f),t.translation) * view;

            mainCamera = &c.camera;
        }
    }
    if(!mainCamera) return;

    Renderer::BeginScene(*mainCamera,view);

    glm::vec3 dlightcol(1.0f);
    glm::vec3 dlights[2];
    dlights[0] = glm::vec3(-1.0f);
    dlights[1] = glm::vec3(1.0f);
    glm::vec3 zero(0.0f);
    for(int i = 0; i < 2; i++){
        m_SceneShader->SetUniform("u_DLights[" + std::to_string(i) + "].direction",ShaderData::FLOAT3,glm::value_ptr(dlights[i]));
        m_SceneShader->SetUniform("u_DLights[" + std::to_string(i) + "].ambient",ShaderData::FLOAT3,glm::value_ptr(dlightcol * 0.1f));
        m_SceneShader->SetUniform("u_DLights[" + std::to_string(i) + "].diffuse",ShaderData::FLOAT3,glm::value_ptr(dlightcol));
        m_SceneShader->SetUniform("u_DLights[" + std::to_string(i) + "].specular",ShaderData::FLOAT3,glm::value_ptr(zero));
    }
    for(auto &e : m_SceneRegistry.group<TransformComponent>(entt::get_t<MeshInstance>())){
        auto [transform, mesh] = m_SceneRegistry.get<TransformComponent,MeshInstance>(e);
        glm::mat4 model = glm::eulerAngleXYZ(glm::radians(transform.rotation.x),glm::radians(transform.rotation.y),glm::radians(transform.rotation.z));
        model = glm::translate(glm::mat4(1.0f),transform.translation) * model;
        model = glm::scale(model,transform.scale);
        int j = 0;
        m_SceneShader->SetUniform("u_Material.diffuse",ShaderData::INT,&j);
        j = 1;
        m_SceneShader->SetUniform("u_Material.specular",ShaderData::INT,&j);
        j = 2;
        m_SceneShader->SetUniform("u_Material.normal",ShaderData::INT,&j);

        for(int i = 0; i < mesh.mesh->submeshes.size(); i++){
            mesh.mesh->submeshes[i].material.parent->diffuseTexture->Bind(0);
            mesh.mesh->submeshes[i].material.parent->specularTexture->Bind(1);
            mesh.mesh->submeshes[i].material.parent->normalTexture->Bind(2);
            Renderer::Submit(m_SceneShader,mesh.mesh->submeshes[i].vertices,model);
        }
    }
    Renderer::EndScene();
}

}