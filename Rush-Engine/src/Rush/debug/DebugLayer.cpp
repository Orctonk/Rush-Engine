#include "DebugLayer.h"

#include "Rush/core/Time.h"
#include "Rush/graphics/Renderer.h"

#include <imgui.h>

namespace Rush {

DebugLayer::DebugLayer(){}
DebugLayer::~DebugLayer(){}
void DebugLayer::OnAttach(){
    for(int i = 0; i < DEBUG_FPS_SAMPLES; i++)
        m_FrameTimes[i] = 0.0;
    m_FrameTimeOffset = 0;
}
void DebugLayer::OnDetach(){}
void DebugLayer::OnUpdate(){
    m_FrameTimes[m_FrameTimeOffset] = Time::Delta();
    m_FrameTimeOffset = (m_FrameTimeOffset + 1) % DEBUG_FPS_SAMPLES;
}
void DebugLayer::OnEvent(Event &e){}
void DebugLayer::OnImguiRender(){
    ImGui::Begin("Debug info");
    
    float tot = 0.0f;
    for(int i = 0; i < DEBUG_FPS_SAMPLES; i++)
        tot += m_FrameTimes[i];
    float avg = tot/DEBUG_FPS_SAMPLES;
    ImGui::Text("Average frame time: %.2f ms/frame (%.1f FPS)", (1000*avg),(1/avg));
    ImGui::PlotLines("Frame times",m_FrameTimes,DEBUG_FPS_SAMPLES,m_FrameTimeOffset);

    ImGui::Separator();
    auto rs = Renderer::GetRenderStats();
    ImGui::Text("3D Renderer statistics");
    ImGui::Text("Draw calls: %d", rs.drawCallCount);
    ImGui::Text("Primitives: %d", rs.primitivesCount);
    ImGui::Text("Programs:   %d", rs.programCount);
    ImGui::Text("Vertices:   %d", rs.vertexCount);
    Renderer::ResetRenderStats();

    ImGui::End();
}

}