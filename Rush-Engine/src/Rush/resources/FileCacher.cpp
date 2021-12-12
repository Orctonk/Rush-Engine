#include "FileCacher.h"

namespace Rush {

FileCache::Path FileCache::s_CacheDir;

void FileCache::EnsureCacheDir() {
    if (!std::filesystem::exists(s_CacheDir))
        std::filesystem::create_directories(s_CacheDir);
}

void FileCache::Init() {
    s_CacheDir = std::filesystem::current_path() / "cache";
}

void FileCache::CacheBinaryFile(const std::string &cacheFileName, const std::vector<uint32_t> &data) {
    EnsureCacheDir();
    std::ofstream cacheFile;
    cacheFile.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    try {
        cacheFile.open(s_CacheDir / cacheFileName, std::ios::out | std::ios::binary);
        cacheFile.write((char *)data.data(), data.size() * 4);
    } catch (std::ofstream::failure &e) {
        RUSH_LOG_WARNING("Failed to create cache file {}'", e.what());
    }
}

bool FileCache::GetCachedBinaryFile(const std::string &cacheFileName, std::vector<uint32_t> &data, const FileChangeTime &lastChange) {
    data.clear();
    if (!std::filesystem::exists(s_CacheDir / cacheFileName))
        return false;
    if (std::filesystem::last_write_time(s_CacheDir / cacheFileName) < lastChange)
        return false;
    std::ifstream cacheFile;
    cacheFile.unsetf(std::ios::skipws);
    cacheFile.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    try {
        cacheFile.open(s_CacheDir / cacheFileName, std::ios::in | std::ios::binary);

        cacheFile.seekg(0, std::ios::end);
        size_t filesize = cacheFile.tellg();
        cacheFile.seekg(0, std::ios::beg);

        data.resize(filesize / 4);
        cacheFile.read((char *)data.data(), filesize);

        return true;
    } catch (std::ofstream::failure &e) {
        RUSH_LOG_WARNING("Failed to open cache file {}'", e.code().message());
    }
    return false;
}

void FileCache::CacheTextFile(const std::string &cacheFileName, const std::string &data) {
    EnsureCacheDir();
    std::ofstream cacheFile;
    cacheFile.exceptions(std::ios_base::failbit);
    try {
        cacheFile.open(s_CacheDir / cacheFileName, std::ios::out);
        cacheFile.write(data.c_str(), data.length());
    } catch (std::ofstream::failure &e) {
        RUSH_LOG_WARNING("Failed to create cache file {}'", e.what());
    }
}

bool FileCache::GetCachedTextFile(const std::string &cacheFileName, std::string &data, const FileChangeTime &lastChange) {
    data.clear();
    if (!std::filesystem::exists(s_CacheDir / cacheFileName))
        return false;
    if (std::filesystem::last_write_time(s_CacheDir / cacheFileName) < lastChange)
        return false;
    std::ifstream cacheFile;
    cacheFile.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    try {
        cacheFile.open(s_CacheDir / cacheFileName, std::ios::in);

        cacheFile.seekg(0, std::ios::end);
        size_t filesize = cacheFile.tellg();
        cacheFile.seekg(0, std::ios::beg);

        data.resize(filesize);
        cacheFile.read((char *)data.data(), filesize);

        return true;
    } catch (std::ofstream::failure &e) {
        RUSH_LOG_WARNING("Failed to open cache file {}'", e.code().message());
    }
    return false;
}

} // namespace Rush
