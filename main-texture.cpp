// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Application Includes
#include "common/textureloader.hpp"
#include "common/controls.hpp"
#include "common/Cube.hpp"
#include "common/Model.hpp"

using namespace glm;

static const GLfloat triangle_vertex[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

GLFWwindow* window;

// TODO: Gambiarra variable :D
GLuint TextureUniform;

#define WIDTH 800
#define HEIGHT 600

std::vector<Model*> models;

int initWindow();
int buildScenery();
void renderAll(Shader shader);

int initWindow() {
	// Initialize GLFW
	glewExperimental = true;
	if(!glfwInit()) {
		std::cout << "Failed to initialize GLFW\n";
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
	if(window == NULL) {
		std::cout << "Failed to open GLFW window.";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // Initialize GLEW
	
	// Initialize GLEW
	glewExperimental=true; // Needed in core profile
	if(glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
		return -1;
	}

	// Key input
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// OpenGL
	// Background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test - Z-Buffer
	glEnable(GL_DEPTH_TEST);
	// Accecpt fragment if it is closer to the camera than the former one
	glDepthFunc(GL_LESS);

	return 0;
}

int buildScenery() {
	// Models path
	//char * path = "models/general/cube/cube_textured.obj";
	//char * path = "models/general/scenery/simplev5.obj";
	// N64
	char * path = "models/N64/OoT/link/YoungLink.obj";
	//char * path = "models/N64/OoT/hyrulefield/hyrulefeild.obj";
	//char * path = "models/N64/OoT/templeoftime/TempleofTime.obj";
	
	Model* model = new Model(path);

	models.push_back(model);
	return 0;
}

void renderAll(Shader shader) {
	for(uint32_t i = 0; i < models.size(); i++){
    	models[i]->draw(shader);
	}

}

int main() {
	int error = 0;
	error = initWindow();

	if(error) {
		std::cout << "ERROR::initWindow()" << std::endl;
		return -1;
	}
	
	// Loading Shaders
	Shader shader("shaders/SimpleVertexShader.vert", "shaders/SimpleFragmentShader.frag");
	GLuint programID = shader.ID;

	if(programID == 0) {
		std::cout << "ERROR::Shader loading" << std::endl;
		return -1;		
	}

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	// TODO: Clean gambiarra ;D
	TextureUniform = TextureID;
	
	// Load models
	buildScenery();

	// Pool until "esc" is pressed
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		shader.use();

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		shader.setMat4("model", ModelMatrix);
		shader.setMat4("view", ViewMatrix);
		shader.setMat4("projection", ProjectionMatrix);

		renderAll(Shader());

		// Swap buffers
		glfwSwapBuffers(window);
		glfwWaitEvents();
	}

}
