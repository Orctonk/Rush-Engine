#type vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

out vec2 uv;

void main() {
    uv = aUV;
    gl_Position = vec4(aPos,0.99, 1.0);
}

#type fragment
#version 330 core

out vec4 FragColor;  
in vec2 uv;

uniform sampler2D gPos;
uniform sampler2D gNorm;
uniform sampler2D gColor;

void main(){
    FragColor = vec4(texture(gColor,uv).aaa,1.0f);
}