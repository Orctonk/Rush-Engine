#include "RenderingAPI.h"

#ifdef RUSH_OPENGL
    #include "Platform/OpenGL/OGLRenderingAPI.h"
#else
    #error "No rendering API provided!"
#endif

namespace Rush{

Unique<RenderingAPI> RenderingAPI::Create(){
    return CreateUnique<OGLRenderingAPI>();
}

}