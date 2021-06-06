#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;

layout (location = 0) out vec3 TexCoord;

layout (std140,binding = 0) uniform SceneData {
    mat4 viewProjection;
    vec3 camPos;
} u_Scene;  

void main(){
    TexCoord = aPos;
    vec4 pos = u_Scene.viewProjection * vec4(aPos + u_Scene.camPos, 1.0);
    gl_Position = pos.xyww;
}

#type fragment
#version 450 core

layout (location = 0) in vec3 TexCoord;

layout (location = 0) out vec4 FragColor;

layout (binding = 3) uniform samplerCube u_Skybox;

void main(){
    FragColor = texture(u_Skybox,TexCoord);
}