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
    mat3 TBN;
} vs_out;

uniform SceneData u_Scene;

void main() {
    vs_out.TexCoord = aTexCoord;
    vec3 T = normalize(vec3(u_Scene.model * vec4(aTangent,0.0)));
    vec3 N = normalize(vec3(u_Scene.model * vec4(aNormal,0.0)));
    vec3 B = cross(N,T);
    vs_out.TBN = mat3(T,B,N);
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
    mat3 TBN;
} fs_in;

out vec4 FragColor;  

uniform Material u_Material;

void main(){
    vec3 normal = texture(u_Material.normal,fs_in.TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.TBN * normal);
    FragColor = vec4(normal,1.0f);
}