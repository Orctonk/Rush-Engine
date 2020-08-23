#include "LineRenderer.h"

#include "Rush/resources/AssetManager.h"

#include <glm/gtc/type_ptr.hpp>

namespace Rush {

struct Vertex{
    glm::vec3 position;
    glm::vec4 color;
};

struct RendererData{
    static const uint32_t BATCH_SIZE = 20000;
    static const uint32_t BATCH_VERTEX_COUNT = BATCH_SIZE * 2;
    static const uint32_t BATCH_INDEX_COUNT = BATCH_SIZE * 2;

    Unique<VertexArray>  rendererVA;
    Shared<VertexBuffer> rendererVB;
    Shared<Shader> lineShader;

    std::array<Vertex,BATCH_VERTEX_COUNT> vertices;

    uint32_t lineSegmentCount;
};

static RendererData s_Data;

Unique<RenderingAPI> LineRenderer::s_API;
LineRenderStats LineRenderer::s_Stats;
glm::mat4 LineRenderer::s_SceneVP;

void LineRenderer::Init(){
    RUSH_PROFILE_FUNCTION();
    s_API = RenderingAPI::Create();

    s_Data.lineSegmentCount = 0;

    s_Data.lineShader = AssetManager::GetShader("res/shaders/lineShader.glsl");

    s_Data.rendererVA = VertexArray::Create();
    s_Data.rendererVB = VertexBuffer::Create(s_Data.BATCH_VERTEX_COUNT * sizeof(Vertex));
    s_Data.rendererVB->SetInterleavedLayout({
        Rush::ShaderData::FLOAT3,
        Rush::ShaderData::FLOAT4
    });
    s_Data.rendererVA->AddVertexBuffer(s_Data.rendererVB);

    uint32_t indices[s_Data.BATCH_INDEX_COUNT];
    for(int i = 0; i < s_Data.BATCH_SIZE; i++){
        indices[i*2 + 0] = i*2 + 0;
        indices[i*2 + 1] = i*2 + 1;
    }

    Shared<IndexBuffer> ib = IndexBuffer::Create(indices,s_Data.BATCH_INDEX_COUNT);
    s_Data.rendererVA->SetIndexBuffer(ib);
}

void LineRenderer::Shutdown(){
    RUSH_PROFILE_FUNCTION();
}

void LineRenderer::BeginScene(glm::mat4 projection, glm::mat4 view){
    RUSH_PROFILE_FUNCTION();
    s_SceneVP = projection * view;
}

void LineRenderer::EndScene(){
    RUSH_PROFILE_FUNCTION();
    Flush();
}

void LineRenderer::Flush(){
    RUSH_PROFILE_FUNCTION();
    if(s_Data.lineSegmentCount == 0)
        return;
    s_Data.rendererVB->BufferData(&s_Data.vertices,s_Data.lineSegmentCount * 4 * sizeof(Vertex));
    s_Data.rendererVA->Bind();
    s_Data.lineShader->Bind();
    s_Data.lineShader->SetUniform("u_ViewProj",ShaderData::MAT4, glm::value_ptr(s_SceneVP));
    s_API->SetOption(BlendMode::Add);
    s_API->SetOption(PolygonMode::Fill);
    s_API->SetOption(CullFace::None);
    s_API->SetOption(DepthTest::Less);
    s_API->DrawIndexed(s_Data.rendererVA, s_Data.lineSegmentCount * 2,Primitive::Line);

    s_Stats.drawCallCount++;
    s_Stats.lineCount += s_Data.lineSegmentCount;
    s_Stats.vertexCount += s_Data.lineSegmentCount * 2;

    s_Data.lineSegmentCount = 0;
}

void LineRenderer::DrawLine(glm::vec2 start, glm::vec2 end, glm::vec4 color){
    DrawLine({start.x,start.y,0.0f},{end.x,end.y,1.0f},color);
}

void LineRenderer::DrawLine(glm::vec3 start, glm::vec3 end, glm::vec4 color){
    DrawLines({start, end},color);
}

void LineRenderer::DrawLines(std::vector<glm::vec3> lines, glm::vec4 color){
    RUSH_PROFILE_FUNCTION();
    uint32_t count = lines.size() - 1;
    if(count + s_Data.lineSegmentCount >= s_Data.BATCH_SIZE)
        Flush();
    
    s_Data.vertices[s_Data.lineSegmentCount * 2].position = lines[0];
    s_Data.vertices[s_Data.lineSegmentCount * 2].color = color;
    s_Data.vertices[s_Data.lineSegmentCount * 2 + 1].position = lines[1];
    s_Data.vertices[s_Data.lineSegmentCount * 2 + 1].color = color;
    s_Data.lineSegmentCount++;
    for(int i = 1; i < count; i++){
        s_Data.vertices[s_Data.lineSegmentCount * 2].position = lines[i];
        s_Data.vertices[s_Data.lineSegmentCount * 2].color = color;
        s_Data.vertices[s_Data.lineSegmentCount * 2 + 1].position = lines[i+1];
        s_Data.vertices[s_Data.lineSegmentCount * 2 + 1].color = color;
        s_Data.lineSegmentCount++;
    }
    
}

void LineRenderer::ResetRenderStats(){
    s_Stats = LineRenderStats();
}


}