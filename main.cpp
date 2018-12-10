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

// Light position
glm::vec3 lightPos(0.0f, 10.0f, 0.0f);

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

Model* lamp;

int buildScenery() {
	// Models path
	//char * path = "models/general/cube/cube_textured.obj";
	//char * path = "models/general/scenery/simplev5.obj";
	// N64
	//char * path = "models/N64/OoT/link/YoungLink.obj";
	//char * path = "models/N64/OoT/hyrulefield/hyrulefeild.obj";
	//char * path = "models/N64/OoT/templeoftime/TempleofTime.obj";
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	
	// Loading objects
	// Zelda
	Model* model1 = new Model("models/N64/OoT/link/YoungLink.obj");
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 2.35f, -3.3f));
	ModelMatrix = glm::rotate(ModelMatrix, 3.1415f, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.03f));
	model1->setModelMatrix(ModelMatrix);

	// Floor
	Model* model3 = new Model("models/general/scenery/simplev5.obj");
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0, 0.0, 0.0));
	model3->setModelMatrix(ModelMatrix);
	
	// Random cube?
	Model* cube = new Model("models/general/cube/cube_textured.obj");

	// Lamp
	lamp = new Model("models/general/cube/cube_textured.obj");
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.2f));
	ModelMatrix = glm::translate(ModelMatrix, lightPos);

	lamp->setModelMatrix(ModelMatrix);
	
	//models.push_back(cube);
	models.push_back(model1);
	models.push_back(model3);

	//models.push_back(lamp);

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
	Shader lightShader("shaders/LightShader.vert", "shaders/LightShader.frag");
//	Shader lightShader("LightShader.vert", "LightShader.frag");
	Shader lampShader("shaders/LightShader.vert", "shaders/LampShader.frag");
	GLuint programID = lightShader.ID;
	if(programID == 0) {
		std::cout << "ERROR::Shader loading" << std::endl;
		return -1;		
	}

	// Load models
	buildScenery();

	// Pool until "esc" is pressed
	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();

		// Use our shader
		lightShader.use();
		
		// Send our transformation to the currently bound shader, in the "MVP" uniform
		lightShader.setMat4("view", ViewMatrix);
		lightShader.setMat4("projection", ProjectionMatrix);

		// Light
		lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightShader.setVec3("light.ambient", 0.4f, 0.4f, 0.4f);
		lightShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		lightShader.setVec3("light.position", lightPos);


		extern glm::vec3 position;
		lightShader.setVec3("viewPos", position);
		renderAll(lightShader);

		lampShader.use();
		lampShader.setMat4("view", ViewMatrix);
		lampShader.setMat4("projection", ProjectionMatrix);

		lamp->draw(lampShader);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwWaitEvents();
	}

}
