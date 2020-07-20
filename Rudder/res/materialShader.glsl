#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal; // the color variable has attribute position 1
layout (location = 2) in vec3 aTangent; // the color variable has attribute position 1
layout (location = 3) in vec2 uvPos;
  
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 FragPos; // output a color to the fragment shader
out vec2 uv;
out mat3 TBN;

void main()
{
    FragPos = vec3(u_Model * vec4(aPos, 1.0)); 
    uv = uvPos;

    vec3 T = normalize(vec3(u_Model * vec4(aTangent,0.0)));
    vec3 N = normalize(vec3(u_Model * vec4(aNormal,0.0)));
    vec3 B = cross(N,T);
    TBN = mat3(T,B,N);
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
}

#type fragment
#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 FragPos;
in vec2 uv;
in mat3 TBN;

uniform vec3 u_CamPos;
uniform Material u_Material;

void main() {   
    gPosition = vec4(FragPos,1.0);

    vec3 normal = texture(u_Material.normal,uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    gNormal = vec4(normalize(TBN * normal), 1.0);

    gColor = vec4(texture(u_Material.diffuse,uv).rgb,texture(u_Material.specular,uv).r);
}