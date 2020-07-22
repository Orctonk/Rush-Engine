#ifndef __ASSETVIEW_H__
#define __ASSETVIEW_H__

#include <string>
#include <vector>

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
    std::vector<std::string> m_DisplayedAssets;

    void RenderAsset(std::string &curDir, const std::string &asset, bool &disp);
    void InvalidateDisplayedAssets();

public:
    bool enabled;

    AssetView();
    ~AssetView();

    void OnImguiRender();
};

#endif // __ASSETVIEW_H__
