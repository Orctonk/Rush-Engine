#include "Rushpch.h"
#include "ModelLoader.h"

#include "Rush/resources/AssetManager.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Rush {

std::string ModelLoader::s_CurDirectory;
Shared<Texture> ModelLoader::s_DefaultColorTexture;
Shared<Texture> ModelLoader::s_DefaultNormalTexture;

RootMesh ModelLoader::LoadModel(const std::string &path){
    RUSH_PROFILE_FUNCTION();
    if(!s_DefaultColorTexture) s_DefaultColorTexture = AssetManager::GetTexture("res/textures/white.png");
    if(!s_DefaultNormalTexture) s_DefaultNormalTexture = AssetManager::GetTexture("res/textures/blue.png");

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);
    
    RootMesh rm;
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        RUSH_LOG_ERROR("Failed to load model '" + path + "'");
        return rm;
    }

    s_CurDirectory = path.substr(0,path.find_last_of("/")+1);

    rm.name = path;

    ProcessNode(rm, scene->mRootNode, scene);
    return rm;
}

void ModelLoader::ProcessNode(RootMesh &root, const aiNode *node, const aiScene *scene){
    RUSH_PROFILE_FUNCTION();
    for(unsigned int i= 0; i < node->mNumMeshes; i++){
        root.submeshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]],scene));
    }
    for(unsigned int i= 0; i < node->mNumChildren; i++){
        ProcessNode(root,node->mChildren[i],scene);
    }
}

SubMesh ModelLoader::ProcessMesh(const aiMesh *mesh, const aiScene *scene){
    RUSH_PROFILE_FUNCTION();
    SubMesh m;
    m.meshName = std::string(mesh->mName.C_Str());
    if(m.meshName.empty()) m.meshName = "Merged mesh";
    m.vertices = VertexArray::Create();
    std::vector<Vertex> vertices;

    vertices.resize(mesh->mNumVertices);

    for(int i = 0; i < mesh->mNumVertices; i++){
        Vertex temp;
        temp.position.x = mesh->mVertices[i].x;
        temp.position.y = mesh->mVertices[i].y;
        temp.position.z = mesh->mVertices[i].z;
        temp.normal.x = mesh->mNormals[i].x;
        temp.normal.y = mesh->mNormals[i].y;
        temp.normal.z = mesh->mNormals[i].z;
        temp.tangent.x = mesh->mTangents[i].x;
        temp.tangent.y = mesh->mTangents[i].y;
        temp.tangent.z = mesh->mTangents[i].z;
        if(mesh->mTextureCoords[0]){
            temp.texture.x = mesh->mTextureCoords[0][i].x;
            temp.texture.y = mesh->mTextureCoords[0][i].y;
        } else {
            temp.texture.x = 0.0f;
            temp.texture.y = 0.0f;
        }
        vertices[i] = temp;
    }  

    Shared<VertexBuffer> meshVB = VertexBuffer::Create((float *)&vertices[0],sizeof(Vertex)*vertices.size());
    meshVB->SetInterleavedLayout({
        ShaderData::FLOAT3,
        ShaderData::FLOAT3,
        ShaderData::FLOAT3,
        ShaderData::FLOAT2,
    });

    m.vertices->AddVertexBuffer(meshVB);

    std::vector<unsigned int> indices;

    for(int i = 0; i < mesh->mNumFaces; i++){
        for(int j = 0; j < mesh->mFaces[i].mNumIndices; j++){
            indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    m.vertices->SetIndexBuffer(IndexBuffer::Create(indices.data(),indices.size()));

    m.material = ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex],scene, m.meshName);
    return m;
}

Shared<Material> ModelLoader::ProcessMaterial(const aiMaterial *material, const aiScene *scene, const std::string &parentMesh){
    RUSH_PROFILE_FUNCTION();
    aiString str;
    std::string matName;
    if(material->Get(AI_MATKEY_NAME,str) != AI_SUCCESS){
        matName = s_CurDirectory + parentMesh + ".mat";
    } else 
        matName = s_CurDirectory + str.C_Str() + ".mat";
    Shared<Material> mat = AssetManager::GetMaterial(matName);
    if(mat != nullptr)
        return mat;

    mat = CreateShared<Material>();
    mat->materialShader = AssetManager::GetShader("res/shaders/materialShader.glsl");
    auto ret = material->GetTexture(aiTextureType_DIFFUSE,0,&str);
    if(ret != AI_FAILURE)   mat->diffuseTexture = AssetManager::GetTexture(s_CurDirectory + str.C_Str());
    else                    mat->diffuseTexture = s_DefaultColorTexture;
    ret = material->GetTexture(aiTextureType_SPECULAR,0,&str);
    if(ret != AI_FAILURE)   mat->specularTexture = AssetManager::GetTexture(s_CurDirectory + str.C_Str());
    else                    mat->specularTexture = s_DefaultColorTexture;
    ret = material->GetTexture(aiTextureType_NORMALS,0,&str);
    if(ret != AI_FAILURE)   mat->normalTexture = AssetManager::GetTexture(s_CurDirectory + str.C_Str());
    else                    mat->normalTexture = s_DefaultNormalTexture;
    ret = aiGetMaterialFloat(material,AI_MATKEY_SHININESS,&mat->shininess);
    if(ret == AI_FAILURE || mat->shininess <= 0.0f)
        mat->shininess = 20.0f;

    Material::Write(mat,Path(matName));
    AssetManager::PutMaterial(matName,mat);

    return mat;
}

}