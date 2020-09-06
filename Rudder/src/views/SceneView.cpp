#include "SceneView.h"

#include <imgui.h>

SceneView::SceneView() { }
SceneView::~SceneView() { }

void SceneView::Init(){
    using namespace Rush;
    enabled = true;
    m_SceneViewSize.x = Application::GetInstance().GetWindow()->GetWidth();
    m_SceneViewSize.y = Application::GetInstance().GetWindow()->GetHeight();
    m_SceneBuffer = Framebuffer::Create({
        Application::GetInstance().GetWindow()->GetWidth(),
        Application::GetInstance().GetWindow()->GetHeight(),
        {16}
    });
}

void SceneView::OnUpdate(Rush::Scene &scene){
    if(!enabled) return;

    m_SceneBuffer->Bind();
    scene.Render();
    m_SceneBuffer->Unbind();
}

void SceneView::OnImguiRender(){
    if(!enabled) return;
    if(ImGui::Begin("Scene",&enabled)) {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        float scale = glm::min(windowSize.x/m_SceneViewSize.x,windowSize.y/m_SceneViewSize.y);
        glm::vec2 offset = glm::vec2((windowSize.x-m_SceneViewSize.x*scale)/2,(windowSize.y-m_SceneViewSize.y*scale)/2);

        ImGui::GetWindowDrawList()->AddImage(
            (void *)(uint64_t)m_SceneBuffer->GetTextures().at(0)->GetID(),
            ImVec2(windowPos.x + offset.x,windowPos.y + offset.y),
            ImVec2(windowPos.x + offset.x + m_SceneViewSize.x*scale, windowPos.y + offset.y + m_SceneViewSize.y*scale),
            ImVec2(0,1),
            ImVec2(1,0)
        );
    }
    ImGui::End();
}