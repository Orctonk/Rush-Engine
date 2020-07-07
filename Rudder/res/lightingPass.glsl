#type vertex
#version 330 core
layout (location = 0) in vec2 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aUV; // the color variable has attribute position 1

out vec2 uv;

void main() {
    uv = aUV;
    gl_Position = vec4(aPos,0.99, 1.0);
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
in vec2 uv;

uniform sampler2D gPos;
uniform sampler2D gNorm;
uniform sampler2D gColor;

uniform DirectionalLight u_DLights[DLIGHT_COUNT];
uniform PointLight u_PLights[PLIGHT_COUNT];
uniform SpotLight u_SLights[SLIGHT_COUNT];
uniform vec3 u_CamPos;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {   
    vec3 result = vec3(0.0);
    vec3 norm = texture(gNorm,uv).rgb;
    vec3 fragPos = texture(gPos,uv).rgb;
    vec3 viewDir = normalize(u_CamPos - fragPos);

    for(int i = 0; i < DLIGHT_COUNT; i++)
        if(u_DLights[i].activated)
            result += CalcDirLight(u_DLights[i],norm,viewDir);

    for(int i = 0; i < PLIGHT_COUNT; i++)
        if(u_PLights[i].activated)
            result += CalcPointLight(u_PLights[i],norm,fragPos,viewDir);
    
    for(int i = 0; i < SLIGHT_COUNT; i++)
        if(u_SLights[i].activated)
            result += CalcSpotLight(u_SLights[i],norm,fragPos,viewDir);
    

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
    vec3 ambient = light.ambient * vec3(texture(gColor, uv).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(gColor, uv).rgb);
    vec3 specular = light.specular * spec * vec3(texture(gColor, uv).a);
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
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(gColor, uv).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(gColor, uv).rgb);
    vec3 specular = light.specular * spec * vec3(texture(gColor, uv).a);
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
    vec3 ambient = light.ambient * vec3(texture(gColor, uv).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(gColor, uv).rgb);
    vec3 specular = light.specular * spec * vec3(texture(gColor, uv).a);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}