#version 130 // Specify which version of GLSL we are using.

// in_Position was bound to attribute index 0("shaderAttribute")
in vec3 in_Position;
in vec2 vertexUV;

out vec2 UV;

// Values that stay constant for the whole mesh
uniform mat4 MVP;

void main() 
{
    gl_Position = MVP * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
    UV = vertexUV;
}
