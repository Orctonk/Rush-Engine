#include "AssetView.h"

#include <Rush.h>
#include <imgui.h>

// TODO: Add proper invalidation through event or such to avoid having to manually refresh

AssetView::AssetView(){
    enabled = true;
    for(int i = 0; i < AssetFilter::Count; i++)
        m_AssetFilters[i] = true;
}

AssetView::~AssetView(){

}

void AssetView::OnImguiRender(){

    // Early out if minimized/closed
    if(!enabled) return;
    if(!ImGui::Begin("Assets", &enabled)){  
        ImGui::End();
        return;
    }

    if(ImGui::Button("Refresh")) InvalidateDisplayedAssets();

    if(ImGui::CollapsingHeader("Filters")){
        if(ImGui::Checkbox("Textures",&m_AssetFilters[AssetFilter::Texture]))   InvalidateDisplayedAssets();
        if(ImGui::Checkbox("Shaders",&m_AssetFilters[AssetFilter::Shader]))     InvalidateDisplayedAssets();
        if(ImGui::Checkbox("Materials",&m_AssetFilters[AssetFilter::Material])) InvalidateDisplayedAssets();
        if(ImGui::Checkbox("Meshes",&m_AssetFilters[AssetFilter::Mesh]))        InvalidateDisplayedAssets();
        ImGui::Separator();
    }

    std::string curDir = "";
    bool curDisp = true;
    for(auto &as : m_DisplayedAssets)
        RenderAsset(curDir,as,curDisp);

    int popCount = 0;
    for(auto c : curDir)
        if(c == '/')
            popCount++;
    if(curDisp == false)
        popCount--;

    for(;popCount > 0; popCount--)
        ImGui::TreePop();

    ImGui::End();
}

void AssetView::RenderAsset(std::string &curDir, const std::string &asset, bool &disp){
    if(!disp && (asset.find(curDir) != asset.npos)) 
        return;

    while(asset.find(curDir) == asset.npos){
        if(disp) ImGui::TreePop();
        curDir = curDir.substr(0,curDir.find_last_of('/',curDir.length()- 2)+1);
        disp = true;
    }

    size_t nextSlash;
    while((nextSlash = asset.find('/',curDir.length())) != asset.npos ){
        std::string dirName = asset.substr(curDir.length(),nextSlash - curDir.length());
        disp = ImGui::TreeNode(dirName.c_str());
        curDir.append(dirName + "/");
        if(!disp) return;
    }

    ImGui::Text((asset.substr(curDir.length(),nextSlash - curDir.length())).c_str());
}

void AssetView::InvalidateDisplayedAssets(){
    m_DisplayedAssets.clear();
    if(m_AssetFilters[AssetFilter::Texture])
        for(auto &as : Rush::AssetManager::GetTextureKeys())
            m_DisplayedAssets.push_back(as);

    if(m_AssetFilters[AssetFilter::Shader])
        for(auto &as : Rush::AssetManager::GetShaderKeys())
            m_DisplayedAssets.push_back(as);

    if(m_AssetFilters[AssetFilter::Material])
        for(auto &as : Rush::AssetManager::GetMaterialKeys())
            m_DisplayedAssets.push_back(as);

    if(m_AssetFilters[AssetFilter::Mesh])
        for(auto &as : Rush::AssetManager::GetMeshKeys())
            m_DisplayedAssets.push_back(as);

    std::sort(m_DisplayedAssets.begin(), m_DisplayedAssets.end());
}