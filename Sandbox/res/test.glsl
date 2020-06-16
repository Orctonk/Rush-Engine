#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal; // the color variable has attribute position 1
layout (location = 2) in vec2 uvPos;
  
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 FragPos; // output a color to the fragment shader
out vec3 Normal; // output a color to the fragment shader
out vec2 uv;

void main()
{
    FragPos = vec3(u_Model * vec4(aPos, 1.0)); 
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    uv = uvPos;
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
}

#type fragment
#version 330 core

#define DLIGHT_COUNT 5
#define SLIGHT_COUNT 5
#define PLIGHT_COUNT 10

struct DirectionalLight {
    bool activated;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    bool activated;
    vec3 position;

    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    bool activated;
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;  
in vec3 FragPos;
in vec3 Normal;
in vec2 uv;

uniform sampler2D tex;
uniform DirectionalLight u_DLights[DLIGHT_COUNT];
uniform PointLight u_PLights[PLIGHT_COUNT];
uniform SpotLight u_SLights[SLIGHT_COUNT];
uniform vec3 u_CamPos;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {   
    vec3 result = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_CamPos - FragPos);

    for(int i = 0; i < DLIGHT_COUNT; i++)
        if(u_DLights[i].activated)
            result += CalcDirLight(u_DLights[i],norm,viewDir);
    for(int i = 0; i < PLIGHT_COUNT; i++)
        if(u_PLights[i].activated)
            result += CalcPointLight(u_PLights[i],norm,FragPos,viewDir);
    
    for(int i = 0; i < SLIGHT_COUNT; i++)
        if(u_SLights[i].activated)
            result += CalcSpotLight(u_SLights[i],norm,FragPos,viewDir);
    

    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(tex, uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(tex, uv));
    vec3 specular = light.specular * spec * vec3(texture(tex, uv));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(tex, uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(tex, uv));
    vec3 specular = light.specular * spec * vec3(texture(tex, uv));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(tex, uv));
    vec3 diffuse = light.diffuse * diff * vec3(texture(tex, uv));
    vec3 specular = light.specular * spec * vec3(texture(tex, uv));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}