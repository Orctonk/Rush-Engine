#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

struct SceneData {
    mat4 model;
    mat4 view;
    mat4 projection;
};  

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_out;

uniform SceneData u_Scene;

void main(){
    vs_out.FragPos = vec3(u_Scene.model * vec4(aPos, 1.0)); 
    vs_out.TexCoord = aTexCoord;

    vec3 T = normalize(vec3(u_Scene.model * vec4(aTangent,0.0)));
    vec3 N = normalize(vec3(u_Scene.model * vec4(aNormal,0.0)));
    vec3 B = cross(N,T);
    vs_out.TBN = mat3(T,B,N);
    gl_Position = u_Scene.projection * u_Scene.view * u_Scene.model * vec4(aPos, 1.0);
}