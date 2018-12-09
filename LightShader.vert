#version 130
// in_Position was bound to attribute index 0("shaderAttribute")
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// Values that stay constant for the whole mesh
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
    gl_Position = projection * view * model * vec4(in_Position, 1.0);
    FragPos = vec3(model * vec4(in_Position, 1.0));
    Normal = in_Normal;
    TexCoords = in_TexCoords;
}
