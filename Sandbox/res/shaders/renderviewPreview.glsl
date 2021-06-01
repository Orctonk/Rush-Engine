#type vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

struct VS_OUT {
    mat3 TBN;
    vec3 FragPos;
    vec3 camPos;
    vec2 TexCoord;
};

layout (location = 0) out VS_OUT vs_out;

layout (std140,binding = 0) uniform SceneData {
    mat4 model;
    mat4 viewProjection;
    vec3 camPos;
} u_Scene;  

void main() {
    vs_out.TexCoord = aTexCoord;
    vec3 T = normalize(vec3(u_Scene.model * vec4(aTangent,0.0)));
    vec3 N = normalize(vec3(u_Scene.model * vec4(aNormal,0.0)));
    vec3 B = cross(N,T);
    vs_out.TBN = mat3(T,B,N);
    vs_out.FragPos = vec3(u_Scene.model * vec4(aPos,1.0));
    vs_out.camPos = u_Scene.camPos;
    gl_Position = u_Scene.viewProjection * u_Scene.model * vec4(aPos,1.0);
}

#type fragment
#version 450 core

struct FS_IN {
    mat3 TBN;
    vec3 FragPos;
    vec3 camPos;
    vec2 TexCoord;
};

layout (location = 0) in FS_IN fs_in;

layout (location = 0) out vec4 FragColor;  

#define DLIGHT_COUNT 5

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (std140,binding = 0) uniform Lights {
    DirectionalLight u_DLights[DLIGHT_COUNT];
};

layout (std140,binding = 1) uniform Material {
    vec4 color;
    float shininess;
} u_Material;

layout (binding = 2) uniform sampler2D diffuseTexture;
layout (binding = 3) uniform sampler2D specularTexture;
layout (binding = 4) uniform sampler2D normalTexture;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main() {   
    vec3 normal = texture(normalTexture,fs_in.TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.TBN * normal);
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(fs_in.camPos - fs_in.FragPos);

    for(int i = 0; i < DLIGHT_COUNT; i++)
        result += CalcDirLight(u_DLights[i],normal,viewDir);

    FragColor = u_Material.color * vec4(result,1.0f);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(diffuseTexture, fs_in.TexCoord).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseTexture, fs_in.TexCoord).rgb);
    vec3 specular = light.specular * spec * vec3(texture(specularTexture, fs_in.TexCoord).r);
    return (ambient + diffuse + specular);
}