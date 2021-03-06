#pragma once

// C++
#include <iostream>
#include <vector>
#include <string>

// GL
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Project
#include "common/Mesh.hpp"
#include "common/Shader.hpp"
#include "common/textureloader.hpp"

using namespace std;

GLuint TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{	
	glm::mat4 model;

	// Model data
	vector<Mesh> meshes;
	string directory;

	// Functions
	void loadModel(char* path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	
public:
	Model(char* path);
	~Model();
	void draw(Shader shader);

	void setModelMatrix(glm::mat4 &model);
};