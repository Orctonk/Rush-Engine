#include "VertexArray.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLVertexArray.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush {

VertexArray::VertexArray() { }

VertexArray::~VertexArray() { }

Unique<VertexArray> VertexArray::Create() {
    return CreateUnique<OGLVertexArray>();
}

}