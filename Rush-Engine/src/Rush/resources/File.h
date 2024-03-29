#ifndef __FILE_H__
#define __FILE_H__

#include "Path.h"
#include "Rush/core/Core.h"

#include <fstream>

namespace Rush {

enum class RUSH_API FileType {
    Directory,
    Regular,
    Link,
    Unknown
};

enum class RUSH_API OpenMode {
    Read,
    Write,
    Append
};

class RUSH_API File {
private:
    Path m_Path;
    FileType m_Type;
    bool m_Hidden;
    bool m_Exists;

public:
    File(Path path);
    ~File();

    bool IsHidden() { return m_Hidden; }
    bool Exists() { return m_Exists; }
    Path GetPath() { return m_Path; }
    FileType GetType() { return m_Type; }
    std::fstream OpenFile(OpenMode mode);
    std::vector<File> OpenDir();

    static File CreateDirectory(Path path);
    static File CreateFile(Path path);
    static File CreateLink(Path path, Path to);
};

} // namespace Rush

#endif // __FILE_H__
