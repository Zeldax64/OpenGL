#pragma once

#include <vector>
#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/textureloader.hpp"

class Cube
{
	GLuint VertexArrayID;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint Texture;
	glm::mat4 ModelMatrix;

public:
	Cube();
	~Cube();

	void draw(GLuint TextureID);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	glm::mat4 getModelMatrix();
};