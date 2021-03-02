#include "Rudderpch.h"
#include "AssetView.h"

#include <Rush.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO: Add proper invalidation through event or such to avoid having to manually refresh

AssetView::AssetView()
: m_SelectedAsset("None"), m_CurrentDir(Rush::Path("res")){
    enabled = true;
    for(int i = 0; i < AssetFilter::Count; i++)
        m_AssetFilters[i] = true;
    m_Dirty = false;
}

AssetView::~AssetView(){

}

void AssetView::OnUpdate(){
    if(m_Dirty)
        RenderMaterialPreview(m_SelectedAsset.GetRawPath());
}

void AssetView::OnImguiRender(){
    // Early out if minimized/closed
    if(!enabled) return;
    if(!ImGui::Begin("Assets", &enabled)){  
        ImGui::End();
        return;
    }

    if(ImGui::Button("Asset root")){
        m_CurrentDir = Rush::Path("res");
    }
    ImGui::SameLine();
    if(ImGui::Button("^")){
        m_CurrentDir = m_CurrentDir.GetParentDirectory();
    }
    ImGui::SameLine();
    ImGui::Text(m_CurrentDir.GetRawPath().c_str());
    ImGui::Separator();
    ImGui::BeginChild("FileSelect");
    Rush::File curDir(m_CurrentDir);

    std::vector<Rush::File> files = curDir.OpenDir();
    std::sort(files.begin(),files.end(),[](Rush::File &f1,Rush::File &f2){
        return f1.GetType() < f2.GetType();
    });
    auto size = ImGui::GetContentRegionAvailWidth();
    int columns = size/64;
    columns = columns < 1 ? 1 : columns;
    ImGui::Columns(columns,NULL,false);
    for(Rush::File f : files){
        ImGui::PushID(f.GetPath().GetRawPath().c_str());
        RenderFile(f);
        ImGui::PopID();
    }
    ImGui::EndChild();

    ImGui::End();
    if(m_SelectedAsset.GetRawPath() != "None"){
        ImGui::Begin("Asset inspector");
        std::string fileext = m_SelectedAsset.GetFileExtension();
        if(fileext == "png" || fileext == "jpg" || fileext == "bmp"){
            Rush::Shared<Rush::Texture> tex = Rush::AssetManager::GetTexture(m_SelectedAsset.GetRawPath());
            auto winSize = ImGui::GetContentRegionAvail();
            float scale = glm::min(winSize.x / tex->GetProperties().width, winSize.y / tex->GetProperties().height);
            ImGui::Image((ImTextureID)(uint64_t)tex->GetID(),ImVec2(tex->GetProperties().width * scale, tex->GetProperties().height * scale),ImVec2(0,1),ImVec2(1,0));
        } else if(fileext == "mat"){
            Rush::Shared<Rush::Material> mat = Rush::AssetManager::GetMaterial(m_SelectedAsset.GetRawPath());
            ImGui::Text("Render mode: ");
            ImGui::SameLine();
            m_Dirty |= ImGui::Combo("##Rendermode", (int *)&mat->mode,"Opaque\0Transparent\0Cutoff\0\0");
            ImGui::Text("Color:       ");
            ImGui::SameLine();
            m_Dirty |= ImGui::ColorEdit4("##Color",glm::value_ptr(mat->color));
            ImGui::Text("Diffuse:     ");
            ImGui::SameLine();
            if(mat->diffuseTexture == nullptr)
                ImGui::Button("None");
            else
                ImGui::Button(mat->diffuseTexture->GetDebugPath().c_str());
            if(ImGui::BeginDragDropTarget()){
                const ImGuiPayload *texPath = ImGui::AcceptDragDropPayload("texture");
                if(texPath != NULL){
                    mat->diffuseTexture = Rush::AssetManager::GetTexture((const char *)texPath->Data);
                    m_Dirty = true;
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::Text("Specular:    ");
            ImGui::SameLine();
            if(mat->specularTexture == nullptr)
                ImGui::Button("None");
            else
                ImGui::Button(mat->specularTexture->GetDebugPath().c_str());
            if(ImGui::BeginDragDropTarget()){
                const ImGuiPayload *texPath = ImGui::AcceptDragDropPayload("texture");
                if(texPath != NULL){
                    mat->specularTexture = Rush::AssetManager::GetTexture((const char *)texPath->Data);
                    m_Dirty = true;
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::Text("Shininess:   ");
            ImGui::SameLine();
            m_Dirty |= ImGui::DragFloat("##Shininess",&mat->shininess);
            ImGui::Text("Normal:      ");
            ImGui::SameLine();
            if(mat->normalTexture == nullptr)
                ImGui::Button("None");
            else
                ImGui::Button(mat->normalTexture->GetDebugPath().c_str());
            if(ImGui::BeginDragDropTarget()){
                const ImGuiPayload *texPath = ImGui::AcceptDragDropPayload("texture");
                if(texPath != NULL){
                    mat->normalTexture = Rush::AssetManager::GetTexture((const char *)texPath->Data);
                    m_Dirty = true;
                }
                ImGui::EndDragDropTarget();
            }
        }

        ImGui::End();
    }
}

void AssetView::RenderFile(Rush::File &file){
    std::string filename = file.GetPath().GetFullFileName();
    if(!file.Exists() || (filename == "..") || (filename == ".")) return;
    if(file.GetType() == Rush::FileType::Directory){
        Rush::Shared<Rush::Texture> folder = Rush::AssetManager::GetTexture("res/textures/gui/folder.png");
        if(ImGui::ImageButton((ImTextureID)(uint64_t)folder->GetID(),ImVec2(64-15,64-16),ImVec2(0,1),ImVec2(1,0)))
            m_CurrentDir = file.GetPath();
        PathDragDrop(file.GetPath(),"directory");
    } else if(file.GetType() == Rush::FileType::Regular){
        Rush::Shared<Rush::Texture> tex;
        std::string ext = file.GetPath().GetFileExtension();
        if(ext == "png" || ext == "bmp" || ext == "jpg"){
            tex = Rush::AssetManager::GetTexture(file.GetPath().GetRawPath());
            if(ImGui::ImageButton((ImTextureID)(uint64_t)tex->GetID(),ImVec2(64-15,64-16),ImVec2(0,1),ImVec2(1,0)))
                m_SelectedAsset = file.GetPath();
            PathDragDrop(file.GetPath(),"texture");
        } else if (ext == "mat"){
            if(m_MaterialPreviews.find(file.GetPath().GetRawPath()) == m_MaterialPreviews.end())
                RenderMaterialPreview(file.GetPath().GetRawPath());
            tex = m_MaterialPreviews[file.GetPath().GetRawPath()];
            if(ImGui::ImageButton((ImTextureID)(uint64_t)tex->GetID(),ImVec2(64-15,64-16),ImVec2(0,1),ImVec2(1,0)))
                m_SelectedAsset = file.GetPath();
            PathDragDrop(file.GetPath(),"material");
        } else if (ext == "obj" || ext == "gltf"){
            tex = Rush::AssetManager::GetTexture("res/textures/gui/model.png");
            if(ImGui::ImageButton((ImTextureID)(uint64_t)tex->GetID(),ImVec2(64-15,64-16),ImVec2(0,1),ImVec2(1,0)))
                m_SelectedAsset = file.GetPath();
            PathDragDrop(file.GetPath(),"mesh");
        }else if (ext == "glsl" || ext == "frag" || ext == "vert"){
            tex = Rush::AssetManager::GetTexture("res/textures/gui/shader.png");
            if(ImGui::ImageButton((ImTextureID)(uint64_t)tex->GetID(),ImVec2(64-15,64-16),ImVec2(0,1),ImVec2(1,0)))
                m_SelectedAsset = file.GetPath();
            PathDragDrop(file.GetPath(),"shader");
        } else{
            tex = Rush::AssetManager::GetTexture("res/textures/gui/file.png");
            if(ImGui::ImageButton((ImTextureID)(uint64_t)tex->GetID(),ImVec2(64-15,64-16),ImVec2(0,1),ImVec2(1,0)))
                m_SelectedAsset = file.GetPath();
        }
    }
    ImGui::TextWrapped(filename.c_str());
    ImGui::NextColumn();
}

void AssetView::RenderMaterialPreview(const std::string &path){
    Rush::Shared<Rush::RootMesh> mesh = Rush::AssetManager::GetMesh("res/models/UVSphere.obj");
    Rush::FramebufferOptions fboptions;
    fboptions.width = 512;
    fboptions.height = 512;
    fboptions.texturePrecisions = {8};
    Rush::Unique<Rush::Framebuffer> FBO = Rush::Framebuffer::Create(fboptions);

    FBO->Bind();
    Rush::Camera cam = Rush::Camera(1.0f,45.0f);
    glm::mat4 view = glm::inverse(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,-2.6f)));
    Rush::Renderer::BeginScene(cam,view);
    Rush::Shared<Rush::Material> mat = Rush::AssetManager::GetMaterial(path);
    mat->Bind();
    mat->materialShader->SetUniform("u_DLightCount", 0);
    mat->materialShader->SetUniform("u_LightCount", 1);
    glm::vec3 trans = glm::vec3(-1.0f,1.0f,1.5f);
    glm::vec4 plv = glm::vec4(trans,0.0f);
    mat->materialShader->SetUniform("u_Lights[0].position_cutoff",Rush::ShaderData::FLOAT4,glm::value_ptr(plv));
    plv = glm::vec4(0.0f,0.0f,0.0f,0.0f);
    mat->materialShader->SetUniform("u_Lights[0].direction_cutoffOuter",Rush::ShaderData::FLOAT4,glm::value_ptr(plv));
    plv = glm::vec4(0.75f,0.75f,0.75f,1.0f);
    mat->materialShader->SetUniform("u_Lights[0].ambient_constant",Rush::ShaderData::FLOAT4,glm::value_ptr(plv));
    plv = glm::vec4(1.0f,1.0f,1.0f,0.35f);
    mat->materialShader->SetUniform("u_Lights[0].diffuse_linear",Rush::ShaderData::FLOAT4,glm::value_ptr(plv));
    plv = glm::vec4(1.0f,1.0f,1.0f,0.44f);
    mat->materialShader->SetUniform("u_Lights[0].specular_quadratic",Rush::ShaderData::FLOAT4,glm::value_ptr(plv));
    for(auto &sm : mesh->submeshes)
        Rush::Renderer::Submit(mat,sm.vertices,glm::mat4(1.0f));

    Rush::Renderer::EndScene();
    FBO->Unbind();

    m_MaterialPreviews[path] = FBO->GetTextures().at(0);
    m_Dirty = false;
}

void AssetView::PathDragDrop(Rush::Path path, const char *name){
    if(ImGui::BeginDragDropSource()){
        ImGui::SetDragDropPayload(name,path.GetRawPath().c_str(),path.GetRawPath().size()+1,ImGuiCond_Once);
        ImGui::Text(path.GetFileName().c_str());
        ImGui::EndDragDropSource();
    }
}
