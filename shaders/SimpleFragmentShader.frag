#version 130 // Specify which version of GLSL we are using.

//precision highp float; // Video card drivers require this line to function properly
// Input data
in vec2 UV;

// Output data
out vec3 color;

// Values that stay constant for the whole mesh
uniform sampler2D myTextureSampler;

void main() 
{
	// Output color = color of the texture at the specified UV
    color = texture(myTextureSampler, UV).rgb;
}
