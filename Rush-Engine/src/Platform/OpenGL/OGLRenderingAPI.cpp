#include "OGLRenderingAPI.h"

#include <glad/glad.h>

namespace Rush {

void OGLRenderingAPI::Init(){
    RUSH_PROFILE_FUNCTION();
    glEnable(GL_DEPTH_TEST);
}

void OGLRenderingAPI::Shutdown(){
    RUSH_PROFILE_FUNCTION();

}

void OGLRenderingAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
    RUSH_PROFILE_FUNCTION();
    glViewport(x,y,width,height);
}

void OGLRenderingAPI::SetClearColor(glm::vec4 color){
    RUSH_PROFILE_FUNCTION();
    glClearColor(color.r,color.g,color.b,color.a);
}

void OGLRenderingAPI::Clear(){
    RUSH_PROFILE_FUNCTION();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderingAPI::DrawIndexed(const Unique<VertexArray> &va){
    RUSH_PROFILE_FUNCTION();
    va->Bind();
    glDrawElements(GL_TRIANGLES,va->GetIndexBuffer()->GetIndexCount(),GL_UNSIGNED_INT,0);
}

}