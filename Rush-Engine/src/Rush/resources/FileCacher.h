#ifndef __FILECACHER_H__
#define __FILECACHER_H__

#include "Rushpch.h"
#include "Rush/core/Core.h"

namespace Rush {

class FileCache {
    using Path = std::filesystem::path;
    using FileChangeTime = std::filesystem::file_time_type;
private:
    static Path s_CacheDir;

    static void EnsureCacheDir();
public:
    static void Init();
    static void SetCacheDirectory(const Path& cacheDir) { s_CacheDir = cacheDir; }
    static void CacheBinaryFile(const std::string &cacheFileName, const std::vector<uint32_t> &data);
    static bool GetCachedBinaryFile(const std::string &cacheFileName, std::vector<uint32_t> &data, const FileChangeTime &lastChange = FileChangeTime::time_point());
    static void CacheTextFile(const std::string &cacheFileName, const std::string &data);
    static bool GetCachedTextFile(const std::string &cacheFileName, std::string &data, const FileChangeTime &lastChange = FileChangeTime::time_point());
};

}

#endif // __FILECACHER_H__
