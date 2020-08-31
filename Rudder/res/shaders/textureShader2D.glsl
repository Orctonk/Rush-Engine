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
    vec4 col = fs_in.Color;
    switch(int(fs_in.TexIndex+0.01)){
        case 0: col *= texture(u_Textures[0],fs_in.TexCoord); break;
        case 1: col *= texture(u_Textures[1],fs_in.TexCoord); break;
        case 2: col *= texture(u_Textures[2],fs_in.TexCoord); break;
        case 3: col *= texture(u_Textures[3],fs_in.TexCoord); break;
        case 4: col *= texture(u_Textures[4],fs_in.TexCoord); break;
        case 5: col *= texture(u_Textures[5],fs_in.TexCoord); break;
        case 6: col *= texture(u_Textures[6],fs_in.TexCoord); break;
        case 7: col *= texture(u_Textures[7],fs_in.TexCoord); break;
        case 8: col *= texture(u_Textures[8],fs_in.TexCoord); break;
        case 9: col *= texture(u_Textures[9],fs_in.TexCoord); break;
        case 10: col *= texture(u_Textures[10],fs_in.TexCoord); break;
        case 11: col *= texture(u_Textures[11],fs_in.TexCoord); break;
        case 12: col *= texture(u_Textures[12],fs_in.TexCoord); break;
        case 13: col *= texture(u_Textures[13],fs_in.TexCoord); break;
        case 14: col *= texture(u_Textures[14],fs_in.TexCoord); break;
        case 15: col *= texture(u_Textures[15],fs_in.TexCoord); break;
        case 16: col *= texture(u_Textures[16],fs_in.TexCoord); break;
        case 17: col *= texture(u_Textures[17],fs_in.TexCoord); break;
        case 18: col *= texture(u_Textures[18],fs_in.TexCoord); break;
        case 19: col *= texture(u_Textures[19],fs_in.TexCoord); break;
        case 20: col *= texture(u_Textures[20],fs_in.TexCoord); break;
        case 21: col *= texture(u_Textures[21],fs_in.TexCoord); break;
        case 22: col *= texture(u_Textures[22],fs_in.TexCoord); break;
        case 23: col *= texture(u_Textures[23],fs_in.TexCoord); break;
        case 24: col *= texture(u_Textures[24],fs_in.TexCoord); break;
        case 25: col *= texture(u_Textures[25],fs_in.TexCoord); break;
        case 26: col *= texture(u_Textures[26],fs_in.TexCoord); break;
        case 27: col *= texture(u_Textures[27],fs_in.TexCoord); break;
        case 28: col *= texture(u_Textures[28],fs_in.TexCoord); break;
        case 29: col *= texture(u_Textures[29],fs_in.TexCoord); break;
        case 30: col *= texture(u_Textures[30],fs_in.TexCoord); break;
        case 31: col *= texture(u_Textures[31],fs_in.TexCoord); break;
    }
    FragColor = col;
}