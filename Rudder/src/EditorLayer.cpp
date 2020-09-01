#include "EditorLayer.h"

#include "views/EditorComponents.h"

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

EditorLayer::EditorLayer() {}

EditorLayer::~EditorLayer() {}

void EditorLayer::OnAttach() {
    using namespace Rush;

    auto globalLight = m_Scene.NewEntity();
    globalLight.AddComponent<EntityName>("Global editor lighting");
    auto &l = globalLight.AddComponent<LightComponent>();
    l.type = LightType::DIRECTIONAL;
    l.ambient = {0.2f,0.2f,0.2f};
    l.diffuse = {0.0f,0.0f,0.0f};
    l.specular = {0.0f,0.0f,0.0f};

    auto testLight = m_Scene.NewEntity();
    testLight.AddComponent<EntityName>("Test point Light");
    testLight.AddComponent<LightComponent>();
    testLight.GetComponent<TransformComponent>().Translate({1.0f,1.0f,1.0f});

    auto model = m_Scene.NewEntity();
    model.AddComponent<MeshInstance>(AssetManager::GetMeshInstance("res/models/backpack/backpack.obj"));
    model.AddComponent<EntityName>("Model Test");

    auto camera = m_Scene.NewEntity();
    camera.AddComponent<EntityName>("Editor camera");
    camera.GetComponent<TransformComponent>().Translate({0.0f,0.0f,3.0f});
    camera.AddComponent<CameraComponent>(false,Rush::Camera(1024.0f/720.0f,45.0f));

    m_RenderViews.Init(camera);
    m_SceneView.Init();

    m_Scene.Save<TransformComponent>(Path("test.scene"));
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

    ImGui::BeginMainMenuBar();
    if(ImGui::BeginMenu("File")){
        if(ImGui::MenuItem("New","Ctrl+N")); // TODO: Implement New button
        if(ImGui::MenuItem("Save","Ctrl+S")); // TODO: Implement Save button
        if(ImGui::MenuItem("Open","Ctrl+O")); // TODO: Implement Open button
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
        ImGui::MenuItem("Debug Info", "", &Rush::DebugLayer::ShowWindow);
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

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