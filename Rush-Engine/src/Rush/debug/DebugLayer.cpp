#include "DebugLayer.h"

#include "Rush/core/Time.h"
#include "Rush/graphics/Renderer.h"
#include "Rush/graphics/LineRenderer.h"
#include "Rush/graphics/Renderer2D.h"

#include <imgui.h>

namespace Rush {

bool DebugLayer::s_ShowWindow;

DebugLayer::DebugLayer(){
    s_ShowWindow = true;
}
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
    if(!s_ShowWindow) return;
    if(ImGui::Begin("Debug info",&s_ShowWindow)){
        float tot = 0.0f;
        for(int i = 0; i < DEBUG_FPS_SAMPLES; i++)
            tot += m_FrameTimes[i];
        float avg = tot/DEBUG_FPS_SAMPLES;
        ImGui::Text("Average frame time: %.2f ms/frame (%.1f FPS)", (1000*avg),(1/avg));
        ImGui::PlotLines("Frame times",m_FrameTimes,DEBUG_FPS_SAMPLES,m_FrameTimeOffset);

        ImGui::Separator();
        ImGui::BeginChild("3DRenderStats",ImVec2(ImGui::GetWindowContentRegionWidth() * 0.33f, 0),true);
        auto rs = Renderer::GetRenderStats();
        ImGui::Text("3D Renderer statistics");
        ImGui::Text("Draw calls: %d", rs.drawCallCount);
        ImGui::Text("Primitives: %d", rs.primitiveCount);
        ImGui::Text("Programs:   %d", rs.programCount);
        ImGui::Text("Vertices:   %d", rs.vertexCount);
        Renderer::ResetRenderStats();
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("2DRenderStats",ImVec2(ImGui::GetWindowContentRegionWidth() * 0.33f, 0),true);
        auto rs2d = Renderer2D::GetRenderStats();
        ImGui::Text("2D Renderer statistics");
        ImGui::Text("Draw calls: %d", rs2d.drawCallCount);
        ImGui::Text("Primitives: %d", rs2d.primitiveCount);
        ImGui::Text("Vertices:   %d", rs2d.vertexCount);
        Renderer2D::ResetRenderStats();
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("LineRenderStats",ImVec2(0, 0),true);
        auto liners = LineRenderer::GetRenderStats();
        ImGui::Text("Line Renderer statistics");
        ImGui::Text("Draw calls: %d", liners.drawCallCount);
        ImGui::Text("Lines: %d", liners.lineCount);
        ImGui::Text("Vertices:   %d", liners.vertexCount);
        LineRenderer::ResetRenderStats();
        ImGui::EndChild();
    }

    ImGui::End();
}

bool DebugLayer::GetShown() { return s_ShowWindow;  }
void DebugLayer::SetShown(bool show) { s_ShowWindow = show; }

}