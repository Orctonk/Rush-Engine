#include "OGLRenderingAPI.h"

#include <glad/glad.h>

namespace Rush {

void OGLRenderingAPI::Init(){
    glEnable(GL_DEPTH_TEST);
}

void OGLRenderingAPI::Shutdown(){

}

void OGLRenderingAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
    glViewport(x,y,width,height);
}

void OGLRenderingAPI::SetClearColor(glm::vec4 color){
    glClearColor(color.r,color.g,color.b,color.a);
}

void OGLRenderingAPI::Clear(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderingAPI::DrawIndexed(const Unique<VertexArray> &va){
    va->Bind();
    glDrawElements(GL_TRIANGLES,va->GetIndexBuffer()->GetIndexCount(),GL_UNSIGNED_INT,0);
}

}