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
    mat4 viewProjection;
    vec3 camPos;
} u_Scene;  

layout (push_constant) uniform ObjectData {
    mat4 model;
} u_Object;

void main() {
    vs_out.TexCoord = aTexCoord;
    gl_Position = u_Scene.viewProjection * u_Object.model * vec4(aPos,1.0);
}

#type fragment
#version 450 core

struct FS_IN {
    vec2 TexCoord;
};

layout (location = 0) in FS_IN fs_in;

layout (location = 0) out vec4 FragColor;  

layout (std140, binding = 2) uniform Material {
    vec4 color;
    float shininess;
} u_Material;

layout (binding = 3) uniform sampler2D diffuseTexture;
layout (binding = 4) uniform sampler2D specularTexture;
layout (binding = 5) uniform sampler2D normalTexture;

void main(){
    vec3 specular = vec3(texture(specularTexture,fs_in.TexCoord).r);
    FragColor = vec4(specular,1.0f);
}