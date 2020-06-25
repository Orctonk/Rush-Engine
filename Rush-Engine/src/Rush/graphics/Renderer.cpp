#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

namespace Rush{

Unique<RenderingAPI> Renderer::s_API = RenderingAPI::Create();
glm::mat4 Renderer::s_SceneVP = glm::mat4();
glm::vec3 Renderer::s_CameraPos = glm::vec3();

void Renderer::Init(){
    s_API = RenderingAPI::Create();
    s_API->Init();
}

void Renderer::Shutdown(){
    s_API->Shutdown();
}

void Renderer::OnResize(uint32_t width, uint32_t height){
    s_API->SetViewport(0,0,width, height);
}

void Renderer::BeginScene(Camera &camera){
    s_SceneVP = camera.GetVPMatrix();
    s_CameraPos = camera.GetPosition();
}

void Renderer::EndScene(){

}

void Renderer::Submit(const Shared<Shader> &shader, const Unique<VertexArray> &va,const glm::mat4 &model) {
    shader->Bind();
    shader->SetUniform("u_ViewProjection", ShaderData::MAT4, glm::value_ptr(s_SceneVP));
    shader->SetUniform("u_Model", ShaderData::MAT4, glm::value_ptr(model));
    shader->SetUniform("u_CamPos", ShaderData::FLOAT3, glm::value_ptr(s_CameraPos));
    s_API->DrawIndexed(va);
}

void Renderer::RenderCube(const Shared<Shader> &shader, const glm::mat4 &model){
    static const Unique<VertexArray> boxVA = VertexArray::Create();
    static bool init = false;
    if(!init){
        const float boxVert[] = {
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f
    };
    const unsigned int boxInd[] = {
        0,1,2,2,3,0,
        4,5,6,6,7,4,
        2,3,7,7,6,3,
        0,1,5,5,4,0,
        1,2,6,6,5,1,
        0,3,7,7,4,0
    };
        auto vb = VertexBuffer::Create(boxVert,sizeof(boxVert));
        vb->SetInterleavedLayout({ShaderData::FLOAT2});
        boxVA->AddVertexBuffer(vb);
        auto ib = IndexBuffer::Create(boxInd,36);
        boxVA->SetIndexBuffer(ib);
        init = true;
    }

    Submit(shader,boxVA,model);
}

void Renderer::RenderQuad(const Shared<Shader> &shader, const glm::vec2 &pos, const glm::vec2 &size){
    static const Unique<VertexArray> quadVA = VertexArray::Create();
    static bool init = false;
    if(!init){
        const float quadVert[] = {
            -1.0f,  1.0f,
            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f,  1.0f
        };
        const unsigned int quadInd[] = {
            0,1,2,
            2,3,0
        };
        auto vb = VertexBuffer::Create(quadVert,sizeof(quadVert));
        vb->SetInterleavedLayout({ShaderData::FLOAT2});
        quadVA->AddVertexBuffer(vb);
        auto ib = IndexBuffer::Create(quadInd,6);
        quadVA->SetIndexBuffer(ib);
        init = true;
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x,pos.y,0.0f));
    model = glm::scale(model, glm::vec3(size.x,size.y,1.0f));

    shader->Bind();
    shader->SetUniform("u_Model",ShaderData::MAT4,glm::value_ptr(model));
    s_API->DrawIndexed(quadVA);
}

void Renderer::RenderTexturedQuad(const Shared<Shader> &shader, const glm::vec2 &pos, const glm::vec2 &size){
    static const Unique<VertexArray> quadVA = VertexArray::Create();
    static bool init = false;
    if(!init){
        const float quadVert[] = {
            -1.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f,  1.0f, 1.0f, 1.0f
        };
        const unsigned int quadInd[] = {
            0,1,2,
            2,3,0
        };
        auto vb = VertexBuffer::Create(quadVert,sizeof(quadVert));
        vb->SetInterleavedLayout({ShaderData::FLOAT2, ShaderData::FLOAT2});
        quadVA->AddVertexBuffer(vb);
        auto ib = IndexBuffer::Create(quadInd,6);
        quadVA->SetIndexBuffer(ib);
        init = true;
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x,pos.y,0.0f));
    model = glm::scale(model, glm::vec3(size.x,size.y,1.0f));

    shader->Bind();
    shader->SetUniform("u_Model",ShaderData::MAT4,glm::value_ptr(model));
    s_API->DrawIndexed(quadVA);
}

}