#version 330 core

out vec4 FragColor;

struct MaterialData {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    
    float shininess;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} fs_in;

uniform vec3 u_CamPos;
uniform MaterialData u_Material;

void main(){
    FragColor = vec4(u_LightCol,1.0);
}