#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

struct VS_OUT {
    vec2 TexCoord;
    mat3 TBN;
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
    vec3 T = normalize(vec3(u_Object.model * vec4(aTangent,0.0)));
    vec3 N = normalize(vec3(u_Object.model * vec4(aNormal,0.0)));
    vec3 B = cross(N,T);
    vs_out.TBN = mat3(T,B,N);
    gl_Position = u_Scene.viewProjection * u_Object.model * vec4(aPos,1.0);
}

#type fragment
#version 450 core

struct FS_IN {
    vec2 TexCoord;
    mat3 TBN;
};

layout (location = 0) in FS_IN fs_in;

layout (std140, binding = 2) uniform Material {
    vec4 color;
    float shininess;
} u_Material;

layout (binding = 3) uniform sampler2D diffuseTexture;
layout (binding = 4) uniform sampler2D specularTexture;
layout (binding = 5) uniform sampler2D normalTexture;

layout (location = 0) out vec4 FragColor; 

void main(){
    vec3 normal = texture(normalTexture,fs_in.TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.TBN * normal);
    FragColor = vec4(normal,1.0f);
}