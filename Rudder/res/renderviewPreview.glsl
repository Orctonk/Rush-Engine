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

struct DirectionalLight {
    vec3 direction;
  
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
uniform vec3 u_CamPos;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main() {   
    vec3 result = vec3(0.0);
    vec3 norm = texture(gNorm,uv).rgb;
    vec3 fragPos = texture(gPos,uv).rgb;
    vec3 viewDir = normalize(u_CamPos - fragPos);

    for(int i = 0; i < DLIGHT_COUNT; i++)
        result += CalcDirLight(u_DLights[i],norm,viewDir);

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