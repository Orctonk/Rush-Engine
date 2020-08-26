#include "Renderer2D.h"

#include <array>
#include "VertexArray.h"
#include "Rush/resources/AssetManager.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

namespace Rush {

struct Vertex{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float texIndex;
};

struct Quad {
    Vertex vertices[4];
};

struct RendererData{
    static const uint32_t BATCH_SIZE = 20000;
    static const uint32_t BATCH_VERTEX_COUNT = BATCH_SIZE * 4;
    static const uint32_t BATCH_INDEX_COUNT = BATCH_SIZE * 6;
    static const uint8_t  MAX_TEXTURES = 32;

    Shared<VertexArray>  rendererVA;
    Shared<VertexBuffer> rendererVB;
    Shared<Texture> whiteTexture;
    Shared<Shader> textureShader;

    std::array<Shared<Texture>,MAX_TEXTURES> textures;
    std::array<Quad,BATCH_SIZE> vertices;

    uint32_t quadCount;
    uint8_t nextTexture;
};

static RendererData s_Data;

RenderStats2D Renderer2D::s_Stats = RenderStats2D();
Unique<RenderingAPI> Renderer2D::s_API;
glm::mat4 Renderer2D::s_SceneVP;
glm::vec3 Renderer2D::s_CameraUp;
glm::vec3 Renderer2D::s_CameraRight;

void Renderer2D::Init(){
    RUSH_PROFILE_FUNCTION();
    s_API = RenderingAPI::Create();
    s_API->Init();

    s_Data.quadCount = 0;
    s_Data.nextTexture = 1;

    s_Data.whiteTexture = AssetManager::GetTexture("res/textures/white.png");
    s_Data.textures[0] = s_Data.whiteTexture;
    s_Data.textureShader = AssetManager::GetShader("res/shaders/textureShader2D.glsl");

    s_Data.rendererVA = VertexArray::Create();
    s_Data.rendererVB = VertexBuffer::Create(s_Data.BATCH_VERTEX_COUNT * sizeof(Vertex));
    s_Data.rendererVB->SetInterleavedLayout({
        Rush::ShaderData::FLOAT3,
        Rush::ShaderData::FLOAT4,
        Rush::ShaderData::FLOAT2,
        Rush::ShaderData::FLOAT
    });
    s_Data.rendererVA->AddVertexBuffer(s_Data.rendererVB);

    uint32_t indices[s_Data.BATCH_INDEX_COUNT];
    for(int i = 0; i < s_Data.BATCH_SIZE; i++){
        indices[i*6 + 0] = i*4 + 0;
        indices[i*6 + 1] = i*4 + 1;
        indices[i*6 + 2] = i*4 + 2;
        indices[i*6 + 3] = i*4 + 2;
        indices[i*6 + 4] = i*4 + 3;
        indices[i*6 + 5] = i*4 + 0;
    }

    Shared<IndexBuffer> ib = IndexBuffer::Create(indices,s_Data.BATCH_INDEX_COUNT);
    s_Data.rendererVA->SetIndexBuffer(ib);

    for(int i = 0; i < s_Data.MAX_TEXTURES; i++)
        s_Data.textureShader->SetUniform("u_Textures[" + std::to_string(i) + "]",ShaderData::INT,&i);
}

void Renderer2D::Shutdown(){

}

void Renderer2D::BeginScene(glm::mat4 projection, glm::mat4 view){
    s_SceneVP = projection * view;
    glm::mat4 camTrans = glm::inverse(view);
    s_CameraUp = glm::vec3(camTrans * glm::vec4(0.0f,1.0f,0.0f,0.0f));
    s_CameraRight = glm::vec3(camTrans * glm::vec4(1.0f,0.0f,0.0f,0.0f));
}

void Renderer2D::EndScene(){
    Flush();
}

void Renderer2D::Flush(){
    if(s_Data.quadCount == 0)
        return;
    glm::vec3 camPos = glm::vec3(glm::inverse(s_SceneVP) * glm::vec4(0.0f,0.0f,0.0f,1.0f));
    std::sort(s_Data.vertices.begin(),s_Data.vertices.begin()+s_Data.quadCount,[camPos](const Quad &q1,const Quad &q2){
        glm::vec3 q1Pos = (q1.vertices[0].position + q1.vertices[1].position + q1.vertices[2].position + q1.vertices[3].position) / 4.0f;
        glm::vec3 q2Pos = (q2.vertices[0].position + q2.vertices[1].position + q2.vertices[2].position + q2.vertices[3].position) / 4.0f;
        return glm::distance2(q1Pos,camPos) > glm::distance2(q2Pos,camPos);
    });
    s_Data.rendererVB->BufferData(&s_Data.vertices,s_Data.quadCount * sizeof(Quad));
    s_Data.rendererVA->Bind();
    s_Data.textureShader->Bind();
    s_Data.textureShader->SetUniform("u_ViewProj",ShaderData::MAT4, glm::value_ptr(s_SceneVP));
    for(int i = 0; i < s_Data.nextTexture; i++)
        s_Data.textures[i]->Bind(i);
    s_API->SetOption(BlendMode::Add);
    s_API->SetOption(PolygonMode::Fill);
    s_API->SetOption(CullFace::None);
    s_API->SetOption(DepthTest::Less);
    s_API->DrawIndexed(s_Data.rendererVA, s_Data.quadCount * 6);

    s_Stats.drawCallCount++;
    s_Stats.primitiveCount += s_Data.quadCount;
    s_Stats.vertexCount += s_Data.quadCount * 4;

    s_Data.quadCount = 0;
    s_Data.nextTexture = 1;
}

void Renderer2D::DrawQuad(glm::mat4 transform, Shared<Texture> texture, glm::vec4 color){
    constexpr glm::vec4 quadVertexPos[4] { 
        {-0.5f,-0.5f, 0.0f, 1.0f},
        { 0.5f,-0.5f, 0.0f, 1.0f},
        { 0.5f, 0.5f, 0.0f, 1.0f},
        {-0.5f, 0.5f, 0.0f, 1.0f}
    };
    constexpr glm::vec2 quadTexCoords[4] { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

    if(s_Data.quadCount >= s_Data.BATCH_SIZE)
        Flush();

    float texIndex = -1.0f;
    for(int i = 0; i < s_Data.nextTexture; i++)
        if(texture == s_Data.textures[i])
            texIndex = i;
    if(texIndex == -1.0f){
        if(s_Data.nextTexture >= s_Data.MAX_TEXTURES)
            Flush();
        texIndex = s_Data.nextTexture;
        s_Data.textures[s_Data.nextTexture++] = texture;
    }

    for(int i = 0; i < 4; i++){
        s_Data.vertices[s_Data.quadCount].vertices[i].position = transform * quadVertexPos[i];
        s_Data.vertices[s_Data.quadCount].vertices[i].color = color;
        s_Data.vertices[s_Data.quadCount].vertices[i].texCoord = quadTexCoords[i];
        s_Data.vertices[s_Data.quadCount].vertices[i].texIndex = texIndex;
    }
    s_Data.quadCount++;
}

void Renderer2D::DrawQuad(glm::mat4 transform, glm::vec4 color) {
    DrawQuad(transform,s_Data.whiteTexture,color);
}

void Renderer2D::DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color){
    DrawQuad({pos.x,pos.y,0.0f},size,color);
}

void Renderer2D::DrawQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color){
    glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f),pos),{size.x,size.y,1.0f});
    DrawQuad(transform,color);
}

void Renderer2D::DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec4 color){
    DrawQuad({pos.x,pos.y,0.0f},size,rotation,color);
}

void Renderer2D::DrawQuad(glm::vec3 pos, glm::vec2 size, float rotation, glm::vec4 color){
    glm::mat4 transform = 
        glm::scale(
            glm::rotate(
                glm::translate(glm::mat4(1.0f),pos),
                glm::radians(rotation),
                glm::vec3(0.0f,0.0f,1.0f)
            ),
        {size.x,size.y,1.0f}
    );
    DrawQuad(transform,color);
}

void Renderer2D::DrawQuad(glm::vec2 pos, glm::vec2 size, Shared<Texture> texture, glm::vec4 color){
    DrawQuad({pos.x,pos.y,0.0f},size,texture,color);
}

void Renderer2D::DrawQuad(glm::vec3 pos, glm::vec2 size, Shared<Texture> texture, glm::vec4 color){
    glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f),pos),{size.x,size.y,1.0f});
    DrawQuad(transform,texture,color);
}

void Renderer2D::DrawQuad(glm::vec2 pos, glm::vec2 size, float rotation, Shared<Texture> texture, glm::vec4 color){
    DrawQuad({pos.x,pos.y,0.0f},size,rotation,texture,color);
}

void Renderer2D::DrawQuad(glm::vec3 pos, glm::vec2 size, float rotation, Shared<Texture> texture, glm::vec4 color){
    glm::mat4 transform = 
        glm::scale(
            glm::rotate(
                glm::translate(glm::mat4(1.0f),pos),
                glm::radians(rotation),
                glm::vec3(0.0f,0.0f,1.0f)
            ),
        {size.x,size.y,1.0f}
    );
    DrawQuad(transform,texture,color);
}

void Renderer2D::DrawBillboard(glm::vec3 pos, glm::vec2 size, glm::vec4 color){
    DrawBillboard(pos,size,s_Data.whiteTexture,color);
}
void Renderer2D::DrawBillboard(glm::vec3 pos, glm::vec2 size, Shared<Texture> texture, glm::vec4 color){
    glm::vec4 quadVertexPos[4] { 
        {-(s_CameraRight*size.x) - (s_CameraUp * size.y) + pos, 1.0f},
        { (s_CameraRight*size.x) - (s_CameraUp * size.y) + pos, 1.0f},
        { (s_CameraRight*size.x) + (s_CameraUp * size.y) + pos, 1.0f},
        {-(s_CameraRight*size.x) + (s_CameraUp * size.y) + pos, 1.0f}
    };
    constexpr glm::vec2 quadTexCoords[4] { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };

    if(s_Data.quadCount >= s_Data.BATCH_SIZE)
        Flush();

    float texIndex = -1.0f;
    for(int i = 0; i < s_Data.nextTexture; i++)
        if(texture == s_Data.textures[i])
            texIndex = i;
    if(texIndex == -1.0f){
        if(s_Data.nextTexture >= s_Data.MAX_TEXTURES)
            Flush();
        texIndex = s_Data.nextTexture;
        s_Data.textures[s_Data.nextTexture++] = texture;
    }

    for(int i = 0; i < 4; i++){
        s_Data.vertices[s_Data.quadCount].vertices[i].position = quadVertexPos[i];
        s_Data.vertices[s_Data.quadCount].vertices[i].color = color;
        s_Data.vertices[s_Data.quadCount].vertices[i].texCoord = quadTexCoords[i];
        s_Data.vertices[s_Data.quadCount].vertices[i].texIndex = texIndex;
    }
    s_Data.quadCount++;
}

void Renderer2D::ResetRenderStats() {
    s_Stats = RenderStats2D();
}
    
    
} // namespace Rush
