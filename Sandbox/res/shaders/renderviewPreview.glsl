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
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
    vec3 camPos;
} vs_out;

uniform SceneData u_Scene;

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
#version 330 core

#define DLIGHT_COUNT 5

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec4 color;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
    vec3 camPos;
} fs_in;

out vec4 FragColor;  

uniform DirectionalLight u_DLights[DLIGHT_COUNT];
uniform Material u_Material;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main() {   
    vec3 normal = texture(u_Material.normal,fs_in.TexCoord).rgb;
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
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, fs_in.TexCoord).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, fs_in.TexCoord).rgb);
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, fs_in.TexCoord).r);
    return (ambient + diffuse + specular);
}