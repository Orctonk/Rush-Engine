#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

struct VS_OUT {
    vec4 Color;
};

layout (location = 0) out VS_OUT vs_out;

layout (std140,binding=0) uniform Camera {
    mat4 u_ViewProj;
};

void main() {
    vec4 pos = u_ViewProj * vec4(aPos,1.0);
    vs_out.Color = aColor;
    gl_Position = pos;
}

#type fragment
#version 450 core

struct FS_IN {
    vec4 Color;
};

layout (location = 0) in FS_IN fs_in;

layout (location = 0) out vec4 FragColor;  

void main() {
    FragColor = fs_in.Color;
}