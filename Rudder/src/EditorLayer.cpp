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
    ApplyStyle();
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
        ImGui::MenuItem("Scene View", "", &m_SceneView.enabled);
        ImGui::MenuItem("Assets", "", &m_AssetView.enabled);
        ImGui::MenuItem("Profiler inspector", "", &m_PRView.enabled);
        ImGui::MenuItem("Exceptions", "", &m_Exceptions.enabled);
        bool show = Rush::DebugLayer::GetShown();
        ImGui::MenuItem("Debug Info", "", &show);
        Rush::DebugLayer::SetShown(show);
        ImGui::EndMenu();
    }
    if (m_Scene.IsPlaying()) {
        if (ImGui::Button(ICON_FA_STOP))
            m_Scene.StopPlay();
    } else if (ImGui::Button(ICON_FA_PLAY)) {
        m_SceneView.enabled = true;
        m_Scene.StartPlay();
    }

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
    m_Exceptions.OnImguiRender();
}

void EditorLayer::AddImGuiFonts() {
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("./res/fonts/OpenSans-Regular.ttf", 16.0f);

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF("./res/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);
    io.Fonts->Build();
}

void EditorLayer::ApplyStyle() {
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;

    /// 0 = FLAT APPEARENCE
    /// 1 = MORE "3D" LOOK
    int is3D = 0;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
    colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
    colors[ImGuiCol_Separator] = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

    style.PopupRounding = 3;

    style.WindowPadding = ImVec2(4, 4);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(6, 2);

    style.ScrollbarSize = 18;

    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = is3D;

    style.WindowRounding = 3;
    style.ChildRounding = 3;
    style.FrameRounding = 3;
    style.ScrollbarRounding = 2;
    style.GrabRounding = 3;

#ifdef IMGUI_HAS_DOCK
    style.TabBorderSize = is3D;
    style.TabRounding = 3;

    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif
}