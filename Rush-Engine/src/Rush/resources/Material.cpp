#include "Rushpch.h"
#include "Material.h"

#include "File.h"
#include "AssetManager.h"

#include <nlohmann/json.hpp>

namespace Rush{

void Material::Bind() {
    materialShader->Bind();
    materialShader->SetUniform("u_Material.diffuse",0);
    diffuseTexture->Bind(0);
    materialShader->SetUniform("u_Material.specular",1);
    specularTexture->Bind(1);
    materialShader->SetUniform("u_Material.normal",2);
    normalTexture->Bind(2);
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
        j["diffuse"] = mat->diffuseTexture->GetDebugPath();
    if(mat->specularTexture != nullptr)
        j["specular"] = mat->specularTexture->GetDebugPath();
    if(mat->normalTexture != nullptr)
        j["normal"] = mat->normalTexture->GetDebugPath();
    File matFile(matPath);
    std::fstream matStream = matFile.OpenFile(OpenMode::Write);
    matStream << j.dump(4);
}

Shared<Material> Material::Load(Path matPath){
    File matFile(matPath);
    if(!matFile.Exists() || matFile.GetType() != FileType::Regular){
        RUSH_LOG_WARNING("Material file " + matPath.GetRawPath() + " does not exist!");
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
    if(j.contains("diffuse")) 
        mat->diffuseTexture = AssetManager::GetTexture(j["diffuse"]);
    if(j.contains("specular")) 
        mat->specularTexture = AssetManager::GetTexture(j["specular"]);
    if(j.contains("normal")) 
        mat->normalTexture = AssetManager::GetTexture(j["normal"]);

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