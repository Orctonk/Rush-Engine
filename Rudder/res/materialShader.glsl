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

#define MAX_LIGHTS 30

struct PackedLight {
    vec4 position_cutoff;
    vec4 direction_cutoffOuter;
    vec4 ambient_constant;
    vec4 diffuse_linear;
    vec4 specular_quadratic;
};

struct Material {
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

uniform PackedLight u_Lights[MAX_LIGHTS];
uniform int u_DLightCount;
uniform int u_LightCount;
uniform Material u_Material;

vec3 CalcDirLight(PackedLight light, vec3 normal, vec3 viewDir);
vec3 CalcOtherLight(PackedLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {   
    vec3 normal = texture(u_Material.normal,fs_in.TexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.TBN * normal);
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(fs_in.camPos - fs_in.FragPos);

    for(int i = 0; i < u_DLightCount; i++)
        result += CalcDirLight(u_Lights[i],normal,viewDir);
    for(int i = u_DLightCount; i < u_LightCount; i++)
        result += CalcOtherLight(u_Lights[i],normal,fs_in.FragPos,viewDir);

    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(PackedLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction_cutoffOuter.xyz);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    // combine results
    vec3 ambient = light.ambient_constant.rgb * vec3(texture(u_Material.diffuse, fs_in.TexCoord).rgb);
    vec3 diffuse = light.diffuse_linear.rgb * diff * vec3(texture(u_Material.diffuse, fs_in.TexCoord).rgb);
    vec3 specular = light.specular_quadratic.rgb * spec * vec3(texture(u_Material.specular, fs_in.TexCoord).r);
    return (ambient + diffuse + specular);
}

vec3 CalcOtherLight(PackedLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position_cutoff.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    // attenuation
    float distance = length(light.position_cutoff.xyz - fragPos);
    float attenuation = 1.0 / (light.ambient_constant.a + light.diffuse_linear.a * distance + light.specular_quadratic.a * (distance * distance));     
    // spotlight intensity
    float intensity;
    if(length(light.direction_cutoffOuter.xyz) < 0.1){
        intensity = 1;
    } else {
        float theta = dot(lightDir, normalize(-light.direction_cutoffOuter.xyz)); 
        float epsilon = light.position_cutoff.a - light.direction_cutoffOuter.a;
        intensity = clamp((theta - light.direction_cutoffOuter.a) / epsilon, 0.0, 1.0);
    }
    // combine results
    vec3 ambient = light.ambient_constant.rgb * vec3(texture(u_Material.diffuse, fs_in.TexCoord).rgb);
    vec3 diffuse = light.diffuse_linear.rgb * diff * vec3(texture(u_Material.diffuse, fs_in.TexCoord).rgb);
    vec3 specular = light.specular_quadratic.rgb * spec * vec3(texture(u_Material.specular, fs_in.TexCoord).r);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}