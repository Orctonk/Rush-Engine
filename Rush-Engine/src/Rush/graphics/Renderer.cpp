#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

namespace Rush{

Unique<RenderingAPI> Renderer::s_API = RenderingAPI::Create();
Renderer::RenderData Renderer::s_Data = Renderer::RenderData(); 
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

void Renderer::BeginScene(Camera &camera, glm::mat4 &view, Shared<Shader> shaderOverride){
    RUSH_PROFILE_FUNCTION();
    s_Data.sceneVP = camera.GetProjection() * glm::inverse(view);
    s_Data.cameraPos = glm::vec3(view * glm::vec4(0.0f,0.0f,0.0f,1.0f));
    s_Data.shaderOverride = shaderOverride;
}

void Renderer::EndScene(){
    RUSH_PROFILE_FUNCTION();

    std::sort(s_Data.keys.begin(),s_Data.keys.end(),[](RenderableKey &k1, RenderableKey &k2){
        if(k1.sortParam & (1UL<<63)){ // K1 Transparent
            if (k2.sortParam & (1UL<<63))
                return k1.sortParam > k2.sortParam;
            return false;
        } else { // K1 Opaque
            return k1.sortParam < k2.sortParam;
        }
    });

    s_API->SetOption(PolygonMode::Fill);
    s_API->SetOption(CullFace::Clockwise);
    s_API->SetOption(DepthTest::Less);
    s_API->SetOption(BlendMode::None);

    bool switched = false;

    for(auto key: s_Data.keys){
        auto &material = s_Data.submittedRenderables[key.renderableIndex].material;
        auto &model = s_Data.submittedRenderables[key.renderableIndex].model;
        auto &va = s_Data.submittedRenderables[key.renderableIndex].va;
        if(!switched && (key.sortParam & (1UL << 63))){
            s_API->SetOption(BlendMode::Alpha);
            switched = true;
        }
        if(s_Data.shaderOverride == nullptr){
            material->Bind();
            material->materialShader->SetUniform("u_Scene.viewProjection", ShaderData::MAT4, glm::value_ptr(s_Data.sceneVP));
            material->materialShader->SetUniform("u_Scene.model", ShaderData::MAT4, glm::value_ptr(model));
            material->materialShader->SetUniform("u_Scene.camPos", ShaderData::FLOAT3, glm::value_ptr(s_Data.cameraPos));
        } else {
            s_Data.shaderOverride->Bind();
            s_Data.shaderOverride->SetUniform("u_Material.diffuse",0);
            material->diffuseTexture->Bind(0);
            s_Data.shaderOverride->SetUniform("u_Material.specular",1);
            material->specularTexture->Bind(1);
            s_Data.shaderOverride->SetUniform("u_Material.normal",2);
            material->normalTexture->Bind(2);
            s_Data.shaderOverride->SetUniform("u_Material.shininess",material->shininess);
            s_Data.shaderOverride->SetUniform("u_Material.color",material->color);
            s_Data.shaderOverride->SetUniform("u_Scene.viewProjection", ShaderData::MAT4, glm::value_ptr(s_Data.sceneVP));
            s_Data.shaderOverride->SetUniform("u_Scene.model", ShaderData::MAT4, glm::value_ptr(model));
            s_Data.shaderOverride->SetUniform("u_Scene.camPos", ShaderData::FLOAT3, glm::value_ptr(s_Data.cameraPos));
        }

        s_API->DrawIndexed(va);

        s_Stats.drawCallCount++;
        uint32_t ic = va->GetIndexBuffer()->GetIndexCount();
        s_Stats.primitiveCount += ic / 3;
        s_Stats.vertexCount += ic;
        s_Stats.programCount++;
    }
    s_Data.submittedRenderables.clear();
    s_Data.keys.clear();
}

void Renderer::ResetRenderStats(){
    s_Stats = RenderStats();
}

void Renderer::Submit(const Shared<Shader> &shader, const Shared<VertexArray> &va,const glm::mat4 &model) {
    RUSH_PROFILE_FUNCTION();
    shader->Bind();
    shader->SetUniform("u_Scene.viewProjection", ShaderData::MAT4, glm::value_ptr(s_Data.sceneVP));
    shader->SetUniform("u_Scene.model", ShaderData::MAT4, glm::value_ptr(model));
    shader->SetUniform("u_Scene.camPos", ShaderData::FLOAT3, glm::value_ptr(s_Data.cameraPos));
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

void Renderer::Submit(Shared<Material> material, const Shared<VertexArray> &va,const glm::mat4 &model) {
    RUSH_PROFILE_FUNCTION();
    s_Data.submittedRenderables.push_back({model,material,va});
    uint64_t sortParam = material->mode == RenderingMode::Opaque ? 0 : (1UL << 63);
    float distance = glm::distance(glm::vec3(model[3]),s_Data.cameraPos);
    sortParam |= ((uint64_t)(distance * 100) & 0x7FFFFFFFFFFFFFFF);
    s_Data.keys.push_back({
        sortParam,
        s_Data.submittedRenderables.size() - 1
    });
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