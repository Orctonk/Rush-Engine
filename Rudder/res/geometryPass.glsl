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

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 uv;

uniform sampler2D tex;
uniform vec3 u_CamPos;

void main() {   
    gPosition = vec4(FragPos,1.0);

    gNormal = vec4(normalize(Normal),1.0);

    gColor = texture(tex,uv);
}

// #type vertex
// #version 330 core
// layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
// layout (location = 1) in vec3 aNormal; // the color variable has attribute position 1
// layout (location = 2) in vec2 uvPos;
  
// uniform mat4 u_Model;
// uniform mat4 u_ViewProjection;

// out vec3 FragPos; // output a color to the fragment shader
// out vec3 Normal; // output a color to the fragment shader
// out vec2 uv;

// void main()
// {
//     FragPos = vec3(u_Model * vec4(aPos, 1.0)); 
//     Normal = mat3(transpose(inverse(u_Model))) * aNormal;
//     uv = uvPos;
//     gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
// }

// #type fragment
// #version 330 core

// //layout (location = 0) out vec3 gPosition;
// //layout (location = 1) out vec3 gNormal;
// //layout (location = 2) out vec4 gColor;

// out vec4 gColor;

// in vec3 FragPos;
// in vec3 Normal;
// in vec2 uv;

// uniform sampler2D tex;
// uniform vec3 u_CamPos;

// void main() {   
//     //gPosition = FragPos;
//     //gNormal = normalize(Normal);

//     gColor = texture(tex,uv);
// }