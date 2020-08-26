#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

namespace Rush{

Unique<RenderingAPI> Renderer::s_API = RenderingAPI::Create();
glm::mat4 Renderer::s_SceneVP = glm::mat4();
glm::vec3 Renderer::s_CameraPos = glm::vec3();
RenderStats Renderer::s_Stats = RenderStats();

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

void Renderer::BeginScene(Camera &camera, glm::mat4 &view){
    RUSH_PROFILE_FUNCTION();
    s_SceneVP = camera.GetProjection() * glm::inverse(view);
    s_CameraPos = glm::vec3(view * glm::vec4(0.0f,0.0f,0.0f,1.0f));
}

void Renderer::EndScene(){
    RUSH_PROFILE_FUNCTION();

}

void Renderer::ResetRenderStats(){
    s_Stats = RenderStats();
}

void Renderer::Submit(const Shared<Shader> &shader, const Shared<VertexArray> &va,const glm::mat4 &model) {
    RUSH_PROFILE_FUNCTION();
    shader->Bind();
    shader->SetUniform("u_Scene.viewProjection", ShaderData::MAT4, glm::value_ptr(s_SceneVP));
    shader->SetUniform("u_Scene.model", ShaderData::MAT4, glm::value_ptr(model));
    shader->SetUniform("u_Scene.camPos", ShaderData::FLOAT3, glm::value_ptr(s_CameraPos));
    s_API->SetOption(PolygonMode::Fill);
    s_API->SetOption(CullFace::None);
    s_API->SetOption(DepthTest::LessOrEqual);
    s_API->SetOption(BlendMode::None);
    s_API->DrawIndexed(va);

    s_Stats.drawCallCount++;
    uint32_t ic = va->GetIndexBuffer()->GetIndexCount();
    s_Stats.primitiveCount += ic / 3;
    s_Stats.vertexCount += ic;
    s_Stats.programCount++;
}

void Renderer::RenderCube(const Shared<Shader> &shader, const glm::mat4 &model){
    RUSH_PROFILE_FUNCTION();
    static const Shared<VertexArray> boxVA = VertexArray::Create();
    static bool init = false;
    if(!init){
        const float boxVert[] = {
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f
    };
    const unsigned int boxInd[] = {
        0,2,1,3,2,0,
        4,5,6,6,7,4,
        2,3,7,7,6,2,
        0,1,5,5,4,0,
        1,2,6,6,5,1,
        0,7,3,7,0,4
    };
        auto vb = VertexBuffer::Create(boxVert,sizeof(boxVert));
        vb->SetInterleavedLayout({ShaderData::FLOAT3});
        boxVA->AddVertexBuffer(vb);
        auto ib = IndexBuffer::Create(boxInd,36);
        boxVA->SetIndexBuffer(ib);
        init = true;
    }

    Submit(shader,boxVA,model);
}
}