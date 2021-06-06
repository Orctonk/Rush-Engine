#ifndef __ASSETVIEW_H__
#define __ASSETVIEW_H__

#include <string>
#include <vector>
#include <Rush.h>

class AssetView {
private:
    struct LightData {
        std::array<PackedLight,15> lights;
        int32_t lightCount;
        int32_t dirLightCount;
    };

    struct DirTree {
        std::string name;
        std::vector<Rush::Shared<DirTree>> children;
    };

    enum AssetFilter{
        Texture = 0,
        Shader = 1,
        Material = 2,
        Mesh = 3,
        Count = 4
    };
    
    Rush::Shared<DirTree> m_DirTree;
    bool m_AssetFilters[AssetFilter::Count];
    bool m_Dirty;
    int m_IconSize;
    Rush::Path m_CurrentDir;
    Rush::Path m_SelectedAsset;
    std::unordered_map<std::string,Rush::Shared<Rush::Texture>> m_MaterialPreviews;
    Rush::Shared<Rush::UniformBuffer> m_LightBuffer;

    void RenderFile(Rush::File &file);
    void RenderMaterialPreview(const std::string &path);
    void RenderDirectories(Rush::Shared<DirTree> tree);
    void PathDragDrop(Rush::Path Path, const char *name);
    Rush::Shared<DirTree> BuildDirTree(Rush::Path path);

public:
    bool enabled;

    AssetView();
    ~AssetView();

    void OnUpdate();
    void OnImguiRender();
};

#endif // __ASSETVIEW_H__
