#include "Path.h"

namespace Rush {

Path Path::s_CWD("/");

Path::Path(std::string raw){
    m_Raw = raw;
#ifdef RUSH_WINDOWS
    for(size_t p = m_Raw.find("/");p != m_Raw.npos; p = m_Raw.find("/",p))
        m_Raw = m_Raw.replace(p,1,"\\");
#else
    for(size_t p = m_Raw.find("\\");p != m_Raw.npos; p = m_Raw.find("\\",p))
        m_Raw = m_Raw.replace(p,1,"/");
#endif
}

Path::~Path(){

}

Path Path::GetParentDirectory() const{
#ifdef RUSH_WINDOWS
    return Path(m_Raw.substr(0,m_Raw.find_last_of('\\')));
#else
    return Path(m_Raw.substr(0,m_Raw.find_last_of('/')));
#endif
}

std::string Path::GetFileName() const{
    std::string filename = GetFullFileName();
    return filename.substr(0,filename.find_last_of('.'));
}

std::string Path::GetFullFileName() const{
#ifdef RUSH_WINDOWS
    return m_Raw.substr(m_Raw.find_last_of('\\') + 1);
#else
    return m_Raw.substr(m_Raw.find_last_of('/') + 1);
#endif
}

std::string Path::GetFileExtension() const{
    return m_Raw.substr(m_Raw.find_last_of('.') + 1);
}
    
}