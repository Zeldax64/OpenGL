#version 130 // Specify which version of GLSL we are using.

//precision highp float; // Video card drivers require this line to function properly

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

/*
// Tutorial 4	
#version 130 // Specify which version of GLSL we are using.

//precision highp float; // Video card drivers require this line to function properly

in vec3 fragmentColor;

// Output data
out vec3 color;

void main() 
{
    color = fragmentColor; //Set colour of each fragment to WHITE
}
*/

/*
#version 130 // Specify which version of GLSL we are using.

//precision highp float; // Video card drivers require this line to function properly

in vec3 fragmentColor;

// Output data
out vec3 color;

void main() 
{
    color = vec4(1.0,0.0,0.0,1.0); //Set colour of each fragment to WHITE
}
*/
/*
#version 130 core

out vec3 color;

void main() {
	color = vec3(1, 0, 0);
}
*/
