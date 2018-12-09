#include "common/controls.hpp"

// Window
extern GLFWwindow* window;

// position
glm::vec3 position = glm::vec3(0, 0, 5);

// horizontal angle : toward -Z
float horizontalAngle = 3.14f;

// vertical angle: 0, look at the horizon
float verticalAngle = 0.0f;

// Initial Field of View
float initialFoV = 45.0f;

float speed = 10.0f; // 3 units per second
float mouseSpeed = 0.05f;

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}

glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

void computeMatricesFromInputs() {
	// glfwGetTime is called only once, the first time this function is called 
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position	
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	// Reset cursor
	glfwSetCursorPos(window, width/2, height/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * deltaTime * float(width/2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(height/2 - ypos);

	// Direction: Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);

	// Moving position
	// Move forward
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}	

	// Move backward
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}

	// Strafe right
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}

	// Strafe left
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;

	// Projection matrix: 45 FOV, 4:3 ratio, displayu range: 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f/3.0f, 0.1f, 100.0f);
	// Camera Matrix
	ViewMatrix = glm::lookAt(
							 position, // Camera position
							 position+direction, // Camera LookAt
							 up // Up vector
				);

	//std::cout << "FPS: " << 1/deltaTime << std::endl;

	lastTime = currentTime;

}