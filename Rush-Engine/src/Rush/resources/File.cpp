#include "Rushpch.h"
#include "File.h"

#ifdef RUSH_LINUX
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <dirent.h>
#else
    #error "No file implementation for platform!"
#endif 

namespace Rush {

File::File(Path path)
: m_Path(path){
    struct stat sb;
    if(lstat(path.GetRawPath().c_str(),&sb) == -1){
        m_Exists = false;
        return;
    }
    m_Exists = true;
    m_Hidden = path.GetFullFileName().at(0) == '.';
    switch(sb.st_mode & S_IFMT){
        case S_IFREG:   m_Type = FileType::Regular;     break;
        case S_IFDIR:   m_Type = FileType::Directory;   break;
        case S_IFLNK:   m_Type = FileType::Link;        break;
        default:        m_Type = FileType::Unknown;
    }
}

File::~File() { }

std::fstream File::OpenFile(OpenMode mode) {
    std::ios_base::openmode om;
    switch(mode){
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
    return std::fstream(m_Path.GetRawPath(),om);
}

std::vector<File> File::OpenDir() {
    std::vector<File> content;
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(m_Path.GetRawPath().c_str())) != NULL){
        while((ent = readdir(dir)) != NULL){
            content.emplace_back(m_Path.GetRawPath() + "/" + ent->d_name);
        }
        closedir(dir);
    }

    return content;
}

File File::CreateDirectory(Path path){
    mkdir(path.GetFullFileName().c_str(),0755);
    return File(path);
}

File File::CreateFile(Path path){
    std::ofstream os(path.GetRawPath());
    return File(path);
}

File File::CreateLink(Path path, Path to){
    symlink(to.GetRawPath().c_str(),path.GetRawPath().c_str());
    return File(path);
}

}