#ifndef __ASSETVIEW_H__
#define __ASSETVIEW_H__

#include <string>

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

    void RenderAsset(std::string parsed, std::string rest);

public:
    bool enabled;

    AssetView();
    ~AssetView();

    void OnImguiRender();
};

#endif // __ASSETVIEW_H__
