#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in mat4 ainstanceMatrix;

out VS_OUT {
    vec4 Color;
} vs_out;

struct SceneData {
    mat4 model;
    mat4 viewProjection;
    vec3 camPos;
};  

uniform SceneData u_Scene;

void main() {
    vs_out.Color = aColor;
    gl_Position = u_Scene.viewProjection * u_Scene.model * ainstanceMatrix * vec4(aPos,1.0);
}

#type fragment
#version 330 core

in VS_OUT {
    vec4 Color;
} fs_in;

out vec4 FragColor;  

void main() {
    FragColor = fs_in.Color;
}