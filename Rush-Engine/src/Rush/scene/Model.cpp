#include "Model.h"

#include "Rush/core/Logger.h"

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

    ProcessNode(scene->mRootNode, scene);
}

Model::~Model(){

}

void Model::ProcessNode(const aiNode *node, const aiScene *scene){
    for(unsigned int i= 0; i < node->mNumMeshes; i++){
        ProcessMesh(scene->mMeshes[node->mMeshes[i]],scene);
    }
    for(unsigned int i= 0; i < node->mNumChildren; i++){
        ProcessNode(node->mChildren[i],scene);
    }
}

void Model::ProcessMesh(const aiMesh *mesh, const aiScene *scene){
    
}

}