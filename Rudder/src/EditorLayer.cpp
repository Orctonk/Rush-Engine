#include "EditorLayer.h"
#include "Rudderpch.h"

#include "widgets/FileBrowser.h"

#include <IconFontCppHeaders/IconsFontAwesome5.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <misc/cpp/imgui_stdlib.h>

EditorLayer::EditorLayer()
    : m_ScenePath("res/scenes/test2.scene") {
}

EditorLayer::~EditorLayer() { }

void EditorLayer::OnAttach() {
    using namespace Rush;
    m_Scene.Load(m_ScenePath);

    auto *reg = m_Scene.GetRegistry();
    if (reg->view<CameraComponent>().empty()) {
        Rush::Entity e1 = m_Scene.NewEntity("Editor Camera");
        e1.AddComponent<CameraComponent>();
    }
    for (auto e : reg->view<CameraComponent>())
        m_RenderViews.Init({ reg, e });

    m_SceneView.Init();
    AddImGuiFonts();
}
void EditorLayer::OnDetach() { }
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
    ImGui::ShowStyleEditor();
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "Ctrl+N")) {
            m_Scene.Load(Rush::Path("res/scenes/new.scene"));
            m_ScenePath = Rush::Path("NewScene.scene");
            auto *reg = m_Scene.GetRegistry();
            for (auto e : reg->view<CameraComponent>())
                m_RenderViews.Init({ reg, e });
        }
        if (ImGui::MenuItem("Save", "Ctrl+S"))
            m_Scene.Save(m_ScenePath);
        if (ImGui::MenuItem("Open", "Ctrl+O"))
            loadScene = true;
        if (ImGui::BeginMenu("Open recent")) {
            ImGui::MenuItem("recent_file_1.scene"); // TODO: Implement Open recent button
            ImGui::MenuItem("recent_file_2.scene");
            ImGui::MenuItem("recent_file_3.scene");
            ImGui::MenuItem("recent_file_4.scene");
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
        ImGui::MenuItem("Undo", "Ctrl+Z");  // TODO: Implement Undo button
        ImGui::MenuItem("Redo", "Ctrl+Y");  // TODO: Implement Redo button
        ImGui::MenuItem("Copy", "Ctrl+C");  // TODO: Implement Copy button
        ImGui::MenuItem("Cut", "Ctrl+X");   // TODO: Implement Cut button
        ImGui::MenuItem("Paste", "Ctrl+V"); // TODO: Implement Paste button
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View")) {
        ImGui::MenuItem("Render preview", "", &m_RenderViews.enabled);
        ImGui::MenuItem("Scene graph", "", &m_SceneGraph.enabled);
        ImGui::MenuItem("Assets", "", &m_AssetView.enabled);
        ImGui::MenuItem("Profiler inspector", "", &m_PRView.enabled);
        bool show = Rush::DebugLayer::GetShown();
        ImGui::MenuItem("Debug Info", "", &show);
        Rush::DebugLayer::SetShown(show);
        ImGui::EndMenu();
    }
    if (ImGui::Button(ICON_FA_PLAY))
        m_Scene.StartPlay();
    if (ImGui::Button(ICON_FA_STOP))
        m_Scene.StopPlay();
    ImGui::EndMainMenuBar();

    if (loadScene) {
        loadBrowser.SetTitle("Open scene...");
        loadBrowser.Open();
    }
    loadBrowser.Render();
    if (loadBrowser.Finished())
        m_Scene.Load(loadBrowser.GetSelectedFile());

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Main window", NULL, flags);
    ImGui::DockSpace(ImGui::GetID("Main window dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();
    ImGui::PopStyleVar();

    m_RenderViews.OnImguiRender();
    m_SceneGraph.OnImguiRender(m_Scene);
    m_AssetView.OnImguiRender();
    m_PRView.OnImguiRender();
    m_SceneView.OnImguiRender();
}

void EditorLayer::AddImGuiFonts() {
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
    io.Fonts->Build();
}
