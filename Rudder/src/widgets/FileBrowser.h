#ifndef __FILEBROWSER_H__
#define __FILEBROWSER_H__

#include <filesystem>
#include <string>
#include <vector>

class FileBrowser {
private:
    static bool s_ShowHidden;

    std::vector<std::filesystem::directory_entry> m_DispDir;
    std::string m_Title;
    std::string m_CWD;
    std::string m_SelectedFile;
    bool m_Finished;

    void SortDirCWD();

public:
    FileBrowser();
    ~FileBrowser();

    void SetTitle(std::string title);
    void SetCWD(std::string title);
    void Open();
    void Render();
    bool Finished();
    std::string GetSelectedFile();
};

#endif // __FILEBROWSER_H__
