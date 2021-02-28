#include "Rushpch.h"
#include "Path.h"

#if defined(RUSH_LINUX)
    #define PATH_DELIM '/'
#elif defined(RUSH_WINDOWS)
    #define PATH_DELIM '\\'
#else
    #error "Paths are not implemented on this platform!"
#endif

namespace Rush {

Path Path::s_CWD("/");

Path::Path(std::string raw){
    m_Raw = raw;
#ifdef RUSH_WINDOWS
    for(size_t p = m_Raw.find("/");p != m_Raw.npos; p = m_Raw.find("/",p))
#else
    for(size_t p = m_Raw.find("\\");p != m_Raw.npos; p = m_Raw.find("\\",p))
#endif
        m_Raw[p] = PATH_DELIM;
}

Path::~Path(){

}

Path Path::GetParentDirectory() const{
    return Path(m_Raw.substr(0,m_Raw.find_last_of(PATH_DELIM)));
}

bool Path::IsAbsolute() const {
    if(!m_Raw.empty())
        return m_Raw[0] == PATH_DELIM;
    return false;
}

Path Path::ToAbsolute() const {
    if(IsAbsolute())
        return *this;
    return GetCWD().m_Raw + m_Raw;
}

std::string Path::GetFileName() const{
    std::string filename = GetFullFileName();
    return filename.substr(0,filename.find_last_of('.'));
}

std::string Path::GetFullFileName() const{
    return m_Raw.substr(m_Raw.find_last_of(PATH_DELIM) + 1);
}

std::string Path::GetFileExtension() const{
    return m_Raw.substr(m_Raw.find_last_of('.') + 1);
}
    
void Path::SetCWD(Path newCWD) { s_CWD = newCWD; }

Path Path::GetCWD() { return s_CWD; };

}