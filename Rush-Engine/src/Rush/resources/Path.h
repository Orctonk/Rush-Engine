#ifndef __PATH_H__
#define __PATH_H__

#include "Rushpch.h"

namespace Rush {

class Path {
private:
    std::string m_Raw;
    static Path s_CWD;

public:
    Path(std::string raw);
    ~Path();

    Path GetParentDirectory();
    std::string GetFileName();
    std::string GetFullFileName();
    std::string GetFileExtension();
    std::string GetRawPath() { return m_Raw; }

    static void SetCWD(Path newCWD) { s_CWD = newCWD; }
    static Path GetCWD() { return s_CWD; };

    bool operator==(const Path &other){ return m_Raw == other.m_Raw; }
};

}

#endif // __PATH_H__
