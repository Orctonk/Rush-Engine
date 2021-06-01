#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

struct VS_OUT {
    vec2 TexCoord;
};

layout (location = 0) out VS_OUT vs_out;

layout (std140,binding = 0) uniform SceneData {
    mat4 model;
    mat4 viewProjection;
    vec3 camPos;
} u_Scene;  

void main() {
    vs_out.TexCoord = aTexCoord;
    gl_Position = u_Scene.viewProjection * u_Scene.model * vec4(aPos,1.0);
}

#type fragment
#version 450 core

struct FS_IN {
    vec2 TexCoord;
};

layout (location = 0) in FS_IN fs_in;

layout (location = 0) out vec4 FragColor;  

layout (std140,binding = 1) uniform Material {
    float shininess;
} u_Material;

layout (binding = 2) uniform sampler2D diffuseTexture;
layout (binding = 3) uniform sampler2D specularTexture;
layout (binding = 4) uniform sampler2D normalTexture;

void main(){
    vec3 albedo = texture(diffuseTexture,fs_in.TexCoord).rgb;
    FragColor = vec4(albedo,1.0f);
}