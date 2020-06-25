#include "OGLFramebuffer.h"

#include "Rush/core/Logger.h"
#include "OGLTexture.h"

#include <glad/glad.h>

namespace Rush {

OGLFramebuffer::OGLFramebuffer(FramebufferOptions options)
:   Framebuffer(options) {
    glGenFramebuffers(1,&m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    std::vector<uint32_t> atts;
    for(int i = 0; i < options.texturePrecisions.size(); i++){
        Shared<OGLTexture> tex = CreateShared<OGLTexture>(options.width,options.height,options.texturePrecisions.at(i));
        m_Textures.push_back(tex);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,tex->GetID(),0);
        atts.push_back(GL_COLOR_ATTACHMENT0+i);
    }
    glDrawBuffers(3,atts.data());

    glGenRenderbuffers(1,&m_DRO);
    glBindRenderbuffer(GL_RENDERBUFFER,m_DRO);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,options.width,options.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_DRO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        RUSH_LOG_ERROR("Framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

OGLFramebuffer::~OGLFramebuffer(){
    glDeleteRenderbuffers(1,&m_DRO);
    glDeleteFramebuffers(1,&m_FBO);
}

void OGLFramebuffer::Bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void OGLFramebuffer::Unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OGLFramebuffer::Blit(Unique<Framebuffer> &other){
    if(other != nullptr){
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ((OGLFramebuffer*)other.get())->m_FBO);
        glBlitFramebuffer(0,0,m_Options.width,m_Options.height,0,0,((OGLFramebuffer*)other.get())->m_Options.width,((OGLFramebuffer*)other.get())->m_Options.height,GL_DEPTH_BUFFER_BIT,GL_NEAREST);
    } else {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0,0,m_Options.width,m_Options.height,0,0,m_Options.width,m_Options.height,GL_DEPTH_BUFFER_BIT,GL_NEAREST);
    }
}
}