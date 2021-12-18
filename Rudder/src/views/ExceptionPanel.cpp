#include "ExceptionPanel.h"
#include <Rush.h>

ExceptionPanel::ExceptionPanel() { }

ExceptionPanel::~ExceptionPanel() { }

void ExceptionPanel::OnImguiRender() {
    if (!enabled) return;

    ImGui::Begin("Exceptions", &enabled);

    auto exceptions = Rush::ExceptionManager::GetExceptionList();
    static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_NoHostExtendX;

    if (ImGui::Button("Clear")) Rush::ExceptionManager::ClearExceptionList();
    ImGui::SameLine();
    ImGui::Text("Exceptions: %d", exceptions.size());

    if (ImGui::BeginTable("Exceptions", 4, flags)) {
        ImGui::TableSetupColumn("Function", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("Line", ImGuiTableColumnFlags_WidthFixed, 40);
        ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (auto &e : exceptions) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", e.function.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d", e.line);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", e.message.c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", e.file.c_str());
        }
        ImGui::EndTable();
    }

    ImGui::End();
}