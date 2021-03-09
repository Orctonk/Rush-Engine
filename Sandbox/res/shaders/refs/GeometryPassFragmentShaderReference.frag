#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColor;

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

void main() {   
    gPosition = vec4(fs_in.FragPos,1.0);

    vec3 normal = texture(u_Material.normal,fs_in.TexCoord).rgb;
    gNormal = vec4(normalize(fs_in.TBN * normal), 1.0);

    gColor = vec4(texture(u_Material.diffuse,fs_in.TexCoord).rgb,texture(u_Material.specular,fs_in.TexCoord).r);
}