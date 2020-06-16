#include "ImguiLayer.h"

#include "Rush/core/Application.h"
#include "Rush/events/Keycodes.h"

#include <examples/imgui_impl_opengl3.h>
#include <imgui.h>

namespace Rush {

ImguiLayer::ImguiLayer() { }

ImguiLayer::~ImguiLayer() { }

void ImguiLayer::OnAttach() {
    ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    // ImGuiStyle& style = ImGui::GetStyle();
    // style.WindowRounding = 0.0f;
    // style.Colors[ImGuiCol_WindowBg].w = 1.0f;

    io.KeyMap[ImGuiKey_Tab]         = RUSH_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]   = RUSH_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow]  = RUSH_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]     = RUSH_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow]   = RUSH_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp]      = RUSH_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown]    = RUSH_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home]        = RUSH_KEY_HOME;
    io.KeyMap[ImGuiKey_End]         = RUSH_KEY_END;
    io.KeyMap[ImGuiKey_Insert]      = RUSH_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete]      = RUSH_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace]   = RUSH_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space]       = RUSH_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter]       = RUSH_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape]      = RUSH_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A]           = RUSH_KEY_A;
    io.KeyMap[ImGuiKey_C]           = RUSH_KEY_C;
    io.KeyMap[ImGuiKey_V]           = RUSH_KEY_V;
    io.KeyMap[ImGuiKey_X]           = RUSH_KEY_X;
    io.KeyMap[ImGuiKey_Y]           = RUSH_KEY_Y;
    io.KeyMap[ImGuiKey_Z]           = RUSH_KEY_Z;

    ImGui_ImplOpenGL3_Init("#version 410");
}

void ImguiLayer::OnDetach() { }

void ImguiLayer::OnUpdate() { 
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::GetInstance();
    io.DisplaySize = ImVec2(app.GetWindow()->GetWidth(), app.GetWindow()->GetHeight());

    io.DeltaTime = (1.0f / 60.0f);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool show = true;
    ImGui::ShowDemoWindow(&show);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiLayer::OnEvent(Event &e) { }

}