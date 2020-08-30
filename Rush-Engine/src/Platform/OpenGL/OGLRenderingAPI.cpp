#include "OGLRenderingAPI.h"

#include <glad/glad.h>

namespace Rush {

void OGLRenderingAPI::Init(){
    RUSH_PROFILE_FUNCTION();
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

void OGLRenderingAPI::SetOption(CullFace culling){
    switch(culling){
        case CullFace::None:
            glDisable(GL_CULL_FACE);
            break;
        case CullFace::Clockwise:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        case CullFace::CounterClockwise:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case CullFace::Both:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT_AND_BACK);
            break;
    }
}

void OGLRenderingAPI::SetOption(PolygonMode polygonMode){
    switch(polygonMode){
        case PolygonMode::Fill:
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            break;
        case PolygonMode::Wireframe:
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            break;
    }
}

void OGLRenderingAPI::SetOption(BlendMode blendMode){
    switch(blendMode){
        case BlendMode::None:
            glDisable(GL_BLEND);
            break;
        case BlendMode::Add:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case BlendMode::Subtract:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBlendEquation(GL_FUNC_SUBTRACT);
            break;
        case BlendMode::Alpha:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
    }
}

void OGLRenderingAPI::SetOption(Multisampling multisampling){
    switch(multisampling){
        case Multisampling::On:
            glEnable(GL_MULTISAMPLE);
            break;
        case Multisampling::Off:
            glDisable(GL_MULTISAMPLE);
            break;
    }
}

void OGLRenderingAPI::SetOption(DepthTest depthTest){
    switch(depthTest){
        case DepthTest::None:
            glDisable(GL_DEPTH_TEST);
            break;
        case DepthTest::Always:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
            break;
        case DepthTest::Never:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_NEVER);
            break;
        case DepthTest::Less:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            break;
        case DepthTest::LessOrEqual:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            break;
        case DepthTest::Greater:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_GREATER);
            break;
        case DepthTest::GreaterOrEqual:
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_GEQUAL);
            break;
    }
}

void OGLRenderingAPI::SetPointSize(float size){
    glPointSize(size);
}

void OGLRenderingAPI::SetLineWidth(float width){
    glLineWidth(width);
}


void OGLRenderingAPI::DrawIndexed(const Shared<VertexArray> &va, Primitive type){
    RUSH_PROFILE_FUNCTION();
    GLenum primType;
    switch(type){
        case Primitive::Point:
            primType = GL_POINTS;
            break;
        case Primitive::Line:
            primType = GL_LINES;
            break;
        case Primitive::Triangle:
            primType = GL_TRIANGLES;
            break;
    }
    va->Bind();
    if(va->GetInstanceCount() == 0)
        glDrawElements(primType,va->GetIndexBuffer()->GetIndexCount(),GL_UNSIGNED_INT,0);
    else 
        glDrawElementsInstanced(primType,va->GetIndexBuffer()->GetIndexCount(),GL_UNSIGNED_INT,0,va->GetInstanceCount());
}

void OGLRenderingAPI::DrawIndexed(const Shared<VertexArray> &va, uint32_t count, Primitive type){
    RUSH_PROFILE_FUNCTION();
    GLenum primType;
    switch(type){
        case Primitive::Point:
            primType = GL_POINTS;
            break;
        case Primitive::Line:
            primType = GL_LINES;
            break;
        case Primitive::Triangle:
            primType = GL_TRIANGLES;
            break;
    }
    va->Bind();
    glDrawElements(primType,count,GL_UNSIGNED_INT,0);
}

}