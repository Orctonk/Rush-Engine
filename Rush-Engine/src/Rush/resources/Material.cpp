#include "Rushpch.h"
#include "Material.h"

#include "File.h"
#include "AssetManager.h"

#include <nlohmann/json.hpp>

namespace Rush{

static Shared<Texture> s_White = nullptr;
static Shared<Texture> s_Blue = nullptr;

uint8_t white[] = { 255,255,255,255 };
uint8_t blue[] = { 128,128,128,128 };

void Material::Bind() {
    materialShader->Bind();

    materialShader->SetUniform("u_Material.diffuse",0);
    if(diffuseTexture)
        diffuseTexture->Bind(0);
    else {
        if (!s_White)
            s_White = Texture::Create(1, 1, TextureFormat::RGBA8, white);
        s_White->Bind(0);
    }

    materialShader->SetUniform("u_Material.specular",1);
    if (specularTexture)
        specularTexture->Bind(1);
    else {
        if (!s_White)
            s_White = Texture::Create(1, 1, TextureFormat::RGBA8, white);
        s_White->Bind(1);
    }

    materialShader->SetUniform("u_Material.normal",2);
    if (normalTexture)
        normalTexture->Bind(2);
    else {
        if (!s_Blue)
            s_Blue = Texture::Create(1, 1, TextureFormat::RGBA8, blue);
        s_Blue->Bind(2);
    }

    materialShader->SetUniform("u_Material.shininess",shininess);
    materialShader->SetUniform("u_Material.color",color);
}

void Material::Write(Shared<Material> mat, Path matPath){
    nlohmann::json j;
    switch(mat->mode){
        case RenderingMode::Opaque:         j["rendermode"] = "opaque"; break;
        case RenderingMode::Transparent:    j["rendermode"] = "transparent"; break;
        case RenderingMode::Cutoff:         j["rendermode"] = "cutoff"; break;
    }
    j["color"] = {mat->color.r,mat->color.g,mat->color.b,mat->color.a};
    j["shininess"] = mat->shininess;
    if(mat->diffuseTexture != nullptr)
        j["diffuse"] = mat->diffuseTexture->GetDebugName();
    if(mat->specularTexture != nullptr)
        j["specular"] = mat->specularTexture->GetDebugName();
    if(mat->normalTexture != nullptr)
        j["normal"] = mat->normalTexture->GetDebugName();
    File matFile(matPath);
    std::fstream matStream = matFile.OpenFile(OpenMode::Write);
    matStream << j.dump(4);
}

Shared<Material> Material::Load(Path matPath){
    File matFile(matPath);
    if(!matFile.Exists() || matFile.GetType() != FileType::Regular){
        RUSH_LOG_WARNING("Material file {} does not exist!", matPath.GetRawPath());
        return nullptr;
    }
    Shared<Material> mat = CreateShared<Material>();
    auto matStream = matFile.OpenFile(OpenMode::Read);

    mat->m_Name = matPath.GetFileName();
    mat->materialShader = AssetManager::GetShader("res/shaders/materialShader.glsl");

    nlohmann::json j;
    matStream >> j;
    if(j.contains("color")){
        mat->color.r = j["color"][0];
        mat->color.g = j["color"][1];
        mat->color.b = j["color"][2];
        mat->color.a = j["color"][3];
    }
    if(j.contains("shininess")) mat->shininess = j["shininess"];
    if (j.contains("diffuse"))
        mat->diffuseTexture = AssetManager::GetTexture(Path(j["diffuse"]));
    if(j.contains("specular")) 
        mat->specularTexture = AssetManager::GetTexture(Path(j["specular"]));
    if(j.contains("normal")) 
        mat->normalTexture = AssetManager::GetTexture(Path(j["normal"]));

    if(j.contains("rendermode")){
        if(j["rendermode"] == "opaque")
            mat->mode == RenderingMode::Opaque;
        if(j["rendermode"] == "transparent")
            mat->mode == RenderingMode::Transparent;
        if(j["rendermode"] == "cutoff")
            mat->mode == RenderingMode::Cutoff;
    }
    return mat;
}

}