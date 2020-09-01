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

    Path GetParentDirectory() const;
    std::string GetFileName() const;
    std::string GetFullFileName() const;
    std::string GetFileExtension() const;
    std::string GetRawPath() const { return m_Raw; }

    static void SetCWD(Path newCWD) { s_CWD = newCWD; }
    static Path GetCWD() { return s_CWD; };

    bool operator==(const Path &other){ return m_Raw == other.m_Raw; }
};

}

#endif // __PATH_H__
