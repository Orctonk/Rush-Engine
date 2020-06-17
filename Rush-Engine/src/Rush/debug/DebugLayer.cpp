#include "DebugLayer.h"

#include "Rush/core/Time.h"

#include <imgui.h>

namespace Rush {

DebugLayer::DebugLayer(){}
DebugLayer::~DebugLayer(){}
void DebugLayer::OnAttach(){
    for(int i = 0; i < 60; i++)
        m_FrameTimes[i] = 0.0;
    m_FrameTimeOffset = 0;
}
void DebugLayer::OnDetach(){}
void DebugLayer::OnUpdate(){
    m_FrameTimes[m_FrameTimeOffset] = Time::Delta();
    m_FrameTimeOffset = (m_FrameTimeOffset + 1) % 60;
}
void DebugLayer::OnEvent(Event &e){}
void DebugLayer::OnImguiRender(){
    bool show = true;
    ImGui::ShowDemoWindow(&show);
    ImGui::Begin("Debug info");
    
    float tot = 0.0f;
    for(int i = 0; i < 60; i++)
        tot += m_FrameTimes[i];
    float avg = tot/60;
    ImGui::Text("Average frame time: %.2f ms/frame (%.1f FPS)", (1000*avg),(1/avg));
    ImGui::PlotLines("Frame times",m_FrameTimes,60,m_FrameTimeOffset);

    ImGui::End();
}

}