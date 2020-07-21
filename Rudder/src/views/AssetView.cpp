#include "AssetView.h"

#include <Rush.h>
#include <imgui.h>

AssetView::AssetView(){
    for(int i = 0; i < AssetFilter::Count; i++)
        m_AssetFilters[i] = true;
}

AssetView::~AssetView(){

}

void AssetView::OnImguiRender(){
    ImGui::ShowDemoWindow(nullptr);   

    ImGui::Begin("Assets");
    ImGui::BeginChild("#AssetFilter");

    if(ImGui::CollapsingHeader("Filters")){
        ImGui::Checkbox("Textures",&m_AssetFilters[AssetFilter::Texture]);
        ImGui::Checkbox("Shaders",&m_AssetFilters[AssetFilter::Shader]);
        ImGui::Checkbox("Materials",&m_AssetFilters[AssetFilter::Material]);
        ImGui::Checkbox("Meshes",&m_AssetFilters[AssetFilter::Mesh]);
    }

    if(m_AssetFilters[AssetFilter::Texture])
        for(auto &as : Rush::AssetManager::GetTextureKeys())
            RenderAsset("",as);

    if(m_AssetFilters[AssetFilter::Shader])
        for(auto &as : Rush::AssetManager::GetShaderKeys())
                RenderAsset("",as);

    if(m_AssetFilters[AssetFilter::Material])
        for(auto &as : Rush::AssetManager::GetMaterialKeys())
            RenderAsset("",as);

    if(m_AssetFilters[AssetFilter::Mesh])
        for(auto &as : Rush::AssetManager::GetMeshKeys())
            RenderAsset("",as);

    ImGui::EndChild();

    ImGui::End();
}

void AssetView::RenderAsset(std::string parsed, std::string rest){
    // TODO: Fix so that directories are rendered as one
    size_t nextSlash = rest.find('/');
    if(nextSlash == rest.npos)
        ImGui::Text(rest.c_str());
    else{
        std::string nodeName = rest.substr(0,nextSlash) + "##" + parsed;
        if(ImGui::TreeNode(nodeName.c_str())){
            RenderAsset(parsed.append(rest.substr(0,nextSlash)),rest.substr(nextSlash+1));
            ImGui::TreePop();
        }
    }
}