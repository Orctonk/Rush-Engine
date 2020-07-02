#include "Model.h"

#include "Rush/core/Logger.h"
#include "Rush/core/ResourceLoader.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Rush {

Model::Model(std::string path){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        RUSH_LOG_ERROR("Failed to load model '" + path + "'");
        return;
    }

    m_Directory = path.substr(0,path.find_last_of("/")+1);

    ProcessNode(scene->mRootNode, scene);
}

Model::~Model(){

}

void Model::ProcessNode(const aiNode *node, const aiScene *scene){
    for(unsigned int i= 0; i < node->mNumMeshes; i++){
        m_Meshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]],scene));
    }
    for(unsigned int i= 0; i < node->mNumChildren; i++){
        ProcessNode(node->mChildren[i],scene);
    }
}

Mesh Model::ProcessMesh(const aiMesh *mesh, const aiScene *scene){
    Mesh m;
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

    m.material = ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex],scene);
    return m;
}

Material Model::ProcessMaterial(const aiMaterial *material, const aiScene *scene){
    Material mat;
    aiString str;
    material->GetTexture(aiTextureType_DIFFUSE,0,&str);
    mat.diffuseTexture = ResourceLoader::LoadTexture(m_Directory + str.C_Str());
    material->GetTexture(aiTextureType_SPECULAR,0,&str);
    mat.specularTexture = ResourceLoader::LoadTexture(m_Directory + str.C_Str());
    //material->GetTexture(aiTextureType_NORMALS,0,&str);
    mat.normalTexture = ResourceLoader::LoadTexture("res/blue.png");
    return mat;
}

}