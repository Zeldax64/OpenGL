#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "common/Shader.hpp"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	string path;
};

struct Material {
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;
};

class Mesh {
	GLuint VAO; // Vertex Attribute Object
	GLuint VBO; // Vertex Buffer Object
	GLuint EBO; // Search

	void setupMesh();

public:
	// Mesh Data
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	//Material mat;
	// Functions
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	~Mesh();
	void draw(Shader shader);
};