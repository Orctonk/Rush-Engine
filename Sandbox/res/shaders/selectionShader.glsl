#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

layout (std140,binding = 0) uniform SceneData {
    mat4 model;
    mat4 viewProjection;
    vec3 camPos;
} u_Scene;

void main() {
    gl_Position = u_Scene.viewProjection * u_Scene.model * vec4(aPos,1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 FragColor;  

layout (binding = 0) uniform ID {
    vec4 u_IDColor;
};

void main(){
    FragColor = u_IDColor;
}