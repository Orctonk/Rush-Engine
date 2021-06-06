#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

layout (std140,binding = 0) uniform SceneData {
    mat4 viewProjection;
    vec3 camPos;
} u_Scene;

layout (push_constant) uniform ObjectData {
    mat4 model;
} u_Object;

void main() {
    gl_Position = u_Scene.viewProjection * u_Object.model * vec4(aPos,1.0);
}

#type fragment
#version 450 core

layout (location = 0) out vec4 FragColor;  

layout (push_constant) uniform ID {
    vec4 color;
} u_ID;

void main(){
    FragColor = u_ID.color;
}