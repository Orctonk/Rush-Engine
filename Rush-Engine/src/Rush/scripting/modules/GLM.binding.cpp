#include "GLM.binding.h"

#include <filesystem>

namespace Rush {
namespace Bindings {

void parseDir(std::vector<std::string> &fileList, std::filesystem::directory_entry ent) {
    if (ent.is_regular_file()) {
        if (ent.path().extension() == ".cs") {
            std::string path = ent.path().string();
            size_t pos;
            while ((pos = path.find('\\')) != std::string::npos) {
                path.replace(pos, 1, "/");
            }
            fileList.push_back(path);
        }
    } else if (ent.is_directory()) {
        std::filesystem::directory_iterator dir(ent);
        for (auto e : dir)
            parseDir(fileList, e);
    }
}

FileList GLM::GetFilenames() {
    using namespace std::filesystem;
    FileList files;

    std::filesystem::directory_entry glmDir(GLMBindingDir + "GlmSharp");
    parseDir(files, glmDir);

    return files;
}

void GLM::BindMethods() { }

} // namespace Bindings
} // namespace Rush