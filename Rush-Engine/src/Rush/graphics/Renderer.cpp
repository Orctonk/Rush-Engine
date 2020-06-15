#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

namespace Rush{

Unique<RenderingAPI> Renderer::s_API = RenderingAPI::Create();
glm::mat4 Renderer::s_SceneVP = glm::mat4();

void Renderer::Init(){
    s_API = RenderingAPI::Create();
    s_API->Init();
}

void Renderer::Shutdown(){
    s_API->Shutdown();
}

void Renderer::OnResize(uint32_t width, uint32_t height){
    s_API->ResizeViewport(width, height);
}

void Renderer::BeginScene(Camera &camera){
    s_SceneVP = camera.GetVPMatrix();
}

void Renderer::EndScene(){

}

void Renderer::Submit(const Shared<Shader> &shader, const Unique<VertexArray> &va, glm::mat4 model) {
    shader->Bind();
    shader->SetUniform("u_ViewProjection", ShaderData::MAT4, glm::value_ptr(s_SceneVP));
    shader->SetUniform("u_Model", ShaderData::MAT4, glm::value_ptr(model));
    s_API->DrawIndexed(va);
}


}