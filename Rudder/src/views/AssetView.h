#ifndef __ASSETVIEW_H__
#define __ASSETVIEW_H__

#include <string>
#include <vector>
#include <Rush.h>

class AssetView {
private:
    enum AssetFilter{
        Texture = 0,
        Shader = 1,
        Material = 2,
        Mesh = 3,
        Count = 4
    };
    
    bool m_AssetFilters[AssetFilter::Count];
    Rush::Path m_CurrentDir;
    Rush::Path m_SelectedAsset;
    std::unordered_map<std::string,Rush::Shared<Rush::Texture>> m_MaterialPreviews;

    void RenderFile(Rush::File &file);
    void RenderMaterialPreview(const std::string &path);
    void PathDragDrop(Rush::Path Path, const char *name);

public:
    bool enabled;

    AssetView();
    ~AssetView();

    void OnImguiRender();
};

#endif // __ASSETVIEW_H__
