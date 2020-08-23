#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out VS_OUT {
    vec4 Color;
} vs_out;

uniform mat4 u_ViewProj;

void main() {
    vec4 pos = u_ViewProj * vec4(aPos,1.0);
    vs_out.Color = aColor;
    gl_Position = pos;
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