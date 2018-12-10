#version 130

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
	sampler2D tex_ambient;
	sampler2D tex_diffuse;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shineness;
};
uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

uniform bool hasTexture;
uniform vec3 lightColor;
uniform vec3 viewPos;
void main() {
	vec3 ambient;
	vec3 diffuse;

	if(hasTexture) {
		ambient = vec3(texture(material.tex_ambient, TexCoords));
		diffuse = vec3(texture(material.tex_diffuse, TexCoords));
	}
	else {
		ambient = material.ambient;
		diffuse = material.diffuse;
	}

	// Ambient
	vec3 Ia = ambient * light.ambient;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 Id = (diff * diffuse) * light.diffuse;
	
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 Is = (material.specular * spec) * light.specular;

	// Result
	vec3 result = (Ia + Id + Is);
	FragColor = vec4(result, 1.0);
}
