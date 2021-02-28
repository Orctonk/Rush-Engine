#ifndef __PATH_H__
#define __PATH_H__

#include "Rush/core/Core.h"

#include <string>

namespace Rush {

class RUSH_API Path {
private:
    std::string m_Raw;
    static Path s_CWD;

public:
    Path(){ Path("/"); }
    Path(std::string raw);
    ~Path();

    Path GetParentDirectory() const;
    bool IsAbsolute() const;
    Path ToAbsolute() const;
    std::string GetFileName() const;
    std::string GetFullFileName() const;
    std::string GetFileExtension() const;
    std::string GetRawPath() const { return m_Raw; }

    static void SetCWD(Path newCWD);
    static Path GetCWD();

    bool operator==(const Path &other){ return m_Raw == other.m_Raw; }
};

}

#endif // __PATH_H__
