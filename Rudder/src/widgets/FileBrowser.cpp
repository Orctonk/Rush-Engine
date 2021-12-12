#include "FileBrowser.h"
#include "Rudderpch.h"

bool FileBrowser::s_ShowHidden = false;

FileBrowser::FileBrowser() {
    m_CWD = "/";
    m_SelectedFile = std::string();
    m_Finished = false;
    m_Title = "Open file";
    SortDirCWD();
}

FileBrowser::~FileBrowser() { }

void FileBrowser::SetTitle(std::string title) {
    m_Title = title;
}

void FileBrowser::SetCWD(std::string cwd) {
    m_CWD = cwd;
    SortDirCWD();
}

void FileBrowser::Open() {
    ImGui::OpenPopup(m_Title.c_str());
}

void FileBrowser::Render() {
    ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);

    if (ImGui::BeginPopupModal(m_Title.c_str())) {

        ImGui::Checkbox("Show hidden", &s_ShowHidden);

        if (ImGui::Button("/"))
            SetCWD("/");
        size_t b = 1;
        size_t e = m_CWD.find("/", b);
        while (b != m_CWD.npos && m_CWD != "/") {
            ImGui::SameLine();
            if (ImGui::Button(m_CWD.substr(b, e - b).c_str())) {
                SetCWD(m_CWD.substr(0, e));
                break;
            }
            b = e == m_CWD.npos ? e : e + 1;
            e = m_CWD.find("/", b);
            if (b != m_CWD.npos) {
                ImGui::SameLine();
                ImGui::Text("/");
            }
        }

        ImGui::BeginChild("FilePicker", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowHeight() - 110), true);

        for (auto &f : m_DispDir) {
            if (!s_ShowHidden && f.path().filename().generic_string().front() == '.')
                continue;
            bool selected = f.path().generic_string() == m_SelectedFile;
            std::string name;
            if (f.is_directory()) {
                name = "[DIR] " + f.path().filename().generic_string();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            } else if (f.is_regular_file()) name = "[FIL] " + f.path().filename().generic_string();
            else name = "[OTH] " + f.path().filename().generic_string();

            if (ImGui::Selectable(name.c_str(), selected, ImGuiSelectableFlags_DontClosePopups)) {
                if (f.is_directory()) {
                    SetCWD(f.path().generic_string());
                    ImGui::PopStyleColor();
                    break;
                } else if (f.is_regular_file()) {
                    m_SelectedFile = f.path().generic_string();
                }
            }

            if (f.is_directory()) {
                ImGui::PopStyleColor();
            }
        }

        ImGui::EndChild();

        ImGui::Separator();
        if (ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Open")) {
            m_Finished = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        ImGui::Text("Selected: %s", m_SelectedFile.c_str());

        ImGui::EndPopup();
    }
}

bool FileBrowser::Finished() {
    return m_Finished;
}

void FileBrowser::SortDirCWD() {
    m_DispDir.clear();
    for (auto &f : std::filesystem::directory_iterator(m_CWD)) {
        m_DispDir.push_back(f);
    }
    std::sort(m_DispDir.begin(), m_DispDir.end(), [](std::filesystem::directory_entry &e1, std::filesystem::directory_entry &e2) {
        if (e1.path().empty() || e2.path().empty()) {
            return false;
        }
        if (e1.is_directory() && !e2.is_directory()) {
            return true;
        } else if (!e1.is_directory() && e2.is_directory()) {
            return false;
        }
        return e1.path().filename().compare(e2.path().filename()) < 0;
    });
}

std::string FileBrowser::GetSelectedFile() {
    m_Finished = false;
    return m_SelectedFile;
}