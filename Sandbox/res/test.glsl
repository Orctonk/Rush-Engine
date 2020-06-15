#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 uvPos;
  
uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 ourColor; // output a color to the fragment shader
out vec2 uv;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
    uv = uvPos;
}

#type fragment
#version 330 core
out vec4 FragColor;  
in vec3 ourColor;
in vec2 uv;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, uv);
}