#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

struct SceneData {
    mat4 model;
    mat4 viewProjection;
    vec3 camPos;
};  

out VS_OUT {
    vec2 TexCoord;
} vs_out;

uniform SceneData u_Scene;

void main() {
    vs_out.TexCoord = aTexCoord;
    gl_Position = u_Scene.viewProjection * u_Scene.model * vec4(aPos,1.0);
}

#type fragment
#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

in VS_OUT {
    vec2 TexCoord;
} fs_in;

out vec4 FragColor;  

uniform Material u_Material;

void main(){
    vec3 specular = vec3(texture(u_Material.specular,fs_in.TexCoord).r);
    FragColor = vec4(specular,1.0f);
}