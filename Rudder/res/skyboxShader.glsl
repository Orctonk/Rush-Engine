#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

struct SceneData {
    mat4 model;
    mat4 viewProjection;
    vec3 camPos;
};  

uniform SceneData u_Scene;

void main(){
    TexCoord = aPos;
    vec4 pos = u_Scene.viewProjection * vec4(aPos + u_Scene.camPos, 1.0);
    gl_Position = pos.xyww;
}

#type fragment
#version 330 core

in vec3 TexCoord;

out vec4 FragColor;

uniform samplerCube u_Skybox;

void main(){
    FragColor = texture(u_Skybox,TexCoord);
}