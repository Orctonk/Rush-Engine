#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

struct SceneData {
    mat4 model;
    mat4 viewProjection;
    vec3 camPos;
};  

uniform SceneData u_Scene;

void main(){
    gl_Position = u_Scene.viewProjection * u_Scene.model * vec4(aPos,1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;

uniform vec3 u_LightCol;

void main(){
    FragColor = vec4(u_LightCol,1.0);
}