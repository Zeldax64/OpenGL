#version 130

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
	sampler2D ambient;
	sampler2D diffuse;
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

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;
void main() {
	// Ambient
	vec3 ambient = vec3(texture(material.ambient, TexCoords)) * light.ambient;

	// Diffuse
	//vec3 norm = normalize(Normal);
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoords))) * light.diffuse;
	
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = (material.specular * spec) * light.specular;

	// Result
	vec3 result = (ambient + diffuse + specular)* objectColor;
	FragColor = vec4(result, 1.0);


	/*
	vec3 result;
	if(norm == vec3(0.0, 0.0, 1)) {
		result = vec3(1.0, 0.0, 0.0); 
	}
	else {
		result = vec3(0.0, 0.0, 0.0);
	}
	result = norm;
	FragColor = vec4(result, 1.0);
	*/
}
