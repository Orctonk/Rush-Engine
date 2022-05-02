#include "File.h"
#include "Rushpch.h"

#include <windows.h>

namespace Rush {

File::File(Path path)
    : m_Path(path) {
    DWORD dwAttrib = GetFileAttributes(path.GetRawPath().c_str());
    if (dwAttrib == INVALID_FILE_ATTRIBUTES) {
        m_Exists = false;
        return;
    }
    m_Exists = true;
    m_Hidden = dwAttrib & FILE_ATTRIBUTE_HIDDEN;
    // TODO: Add more robust file type checks
    if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)
        m_Type = FileType::Directory;
    else if (dwAttrib & FILE_ATTRIBUTE_REPARSE_POINT)
        m_Type = FileType::Link;
    else
        m_Type = FileType::Regular;
}

File::~File() { }

std::fstream File::OpenFile(OpenMode mode) {
    std::ios_base::openmode om;
    switch (mode) {
        case OpenMode::Read:
            om = std::ios_base::in;
            break;
        case OpenMode::Append:
            om = std::ios_base::out | std::ios_base::ate;
            break;
        case OpenMode::Write:
            om = std::ios_base::out | std::ios_base::trunc;
            break;
    }
    return std::fstream(m_Path.GetRawPath(), om);
}

std::vector<File> File::OpenDir() {
    std::vector<File> content;
    WIN32_FIND_DATA found;
    HANDLE findHandle = FindFirstFile((m_Path.GetRawPath() + "\\*").c_str(), &found);
    if (findHandle != INVALID_HANDLE_VALUE) {
        do {
            content.emplace_back(m_Path.GetRawPath() + "\\" + found.cFileName);
        } while (FindNextFile(findHandle, &found));
        FindClose(findHandle);
    }
    return content;
}

#undef CreateDirectory
File File::CreateDirectory(Path path) {
    CreateDirectoryA(path.GetRawPath().c_str(), NULL);
    return File(path);
}

#undef CreateFile
File File::CreateFile(Path path) {
    std::ofstream os(path.GetRawPath());
    return File(path);
}

File File::CreateLink(Path path, Path to) {
    CreateSymbolicLink(path.GetRawPath().c_str(), to.GetRawPath().c_str(), 0);
    return File(path);
}

} // namespace Rush