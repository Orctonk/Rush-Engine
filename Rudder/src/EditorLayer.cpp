#include "Rudderpch.h"
#include "EditorLayer.h"

#include "widgets/FileBrowser.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <misc/cpp/imgui_stdlib.h>

EditorLayer::EditorLayer()
: m_ScenePath("res/scenes/test.scene") {}

EditorLayer::~EditorLayer() {}

void EditorLayer::OnAttach() {
    using namespace Rush;
    m_Scene.Load(m_ScenePath);

    auto *reg = m_Scene.GetRegistry();
    for(auto e : reg->view<CameraComponent>()){
        m_RenderViews.Init({reg,e});
    }

    m_SceneView.Init();

}
void EditorLayer::OnDetach() {}
void EditorLayer::OnUpdate() {
    m_Scene.OnUpdate();
    m_RenderViews.OnUpdate(m_Scene);
    m_SceneView.OnUpdate(m_Scene);
    m_AssetView.OnUpdate();
}

void EditorLayer::OnEvent(Rush::Event &e) {
    m_RenderViews.OnEvent(e);
    m_SceneGraph.OnEvent(e);
    m_PRView.OnEvent(e);
}
void EditorLayer::OnImguiRender() {
    static FileBrowser loadBrowser;
    bool loadScene = false;
    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("File")){
        if(ImGui::MenuItem("New","Ctrl+N")){
            m_Scene.Load(Rush::Path("res/scenes/new.scene"));
            m_ScenePath = Rush::Path("NewScene.scene");
            auto *reg = m_Scene.GetRegistry();
            for(auto e : reg->view<CameraComponent>()){
                m_RenderViews.Init({reg,e});
            }
        }
        if(ImGui::MenuItem("Save","Ctrl+S"))
            m_Scene.Save(m_ScenePath);
        if(ImGui::MenuItem("Open","Ctrl+O")){
            loadScene = true;
        }
        if(ImGui::BeginMenu("Open recent")){
            if(ImGui::MenuItem("recent_file_1.scene")); // TODO: Implement Open recent button
            if(ImGui::MenuItem("recent_file_2.scene"));
            if(ImGui::MenuItem("recent_file_3.scene"));
            if(ImGui::MenuItem("recent_file_4.scene"));
            ImGui::EndMenu();
        } 
        ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("Edit")){
        if(ImGui::MenuItem("Undo","Ctrl+Z")); // TODO: Implement Undo button
        if(ImGui::MenuItem("Redo","Ctrl+Y")); // TODO: Implement Redo button
        if(ImGui::MenuItem("Copy","Ctrl+C")); // TODO: Implement Copy button
        if(ImGui::MenuItem("Cut","Ctrl+X")); // TODO: Implement Cut button
        if(ImGui::MenuItem("Paste","Ctrl+V")); // TODO: Implement Paste button
        ImGui::EndMenu();
    }
    if(ImGui::BeginMenu("View")){
        ImGui::MenuItem("Render preview", "", &m_RenderViews.enabled);
        ImGui::MenuItem("Scene graph", "", &m_SceneGraph.enabled);
        ImGui::MenuItem("Assets", "", &m_AssetView.enabled);
        ImGui::MenuItem("Profiler inspector", "", &m_PRView.enabled);
        bool show = Rush::DebugLayer::GetShown();
        ImGui::MenuItem("Debug Info", "", &show);
        Rush::DebugLayer::SetShown(show);
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
    
    if(loadScene){
        loadBrowser.SetTitle("Open scene...");
        loadBrowser.Open();
    }
    loadBrowser.Render();
    if(loadBrowser.Finished()){
        m_Scene.Load(loadBrowser.GetSelectedFile());
    }

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus 
        | ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f));
    ImGui::Begin("Main window",NULL,flags);
    ImGui::DockSpace(ImGui::GetID("Main window dockspace"),ImVec2(0.0f,0.0f),ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
    ImGui::PopStyleVar();

    m_RenderViews.OnImguiRender();
    m_SceneGraph.OnImguiRender(m_Scene);
    m_AssetView.OnImguiRender();
    m_PRView.OnImguiRender();
    m_SceneView.OnImguiRender();
}