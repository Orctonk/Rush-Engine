#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

out VS_OUT {
    vec2 TexCoord;
    vec4 Color;
    float TexIndex;
} vs_out;

uniform mat4 u_ViewProj;

void main() {
    vs_out.TexCoord = aTexCoord;
    vs_out.Color = aColor;
    vs_out.TexIndex = aTexIndex;
    gl_Position = u_ViewProj * vec4(aPos,1.0);
}

#type fragment
#version 330 core

in VS_OUT {
    vec2 TexCoord;
    vec4 Color;
    float TexIndex;
} fs_in;

uniform sampler2D u_Textures[32];

out vec4 FragColor;  

void main() {
    FragColor = fs_in.Color * texture(u_Textures[int(fs_in.TexIndex+0.01)],fs_in.TexCoord);
}