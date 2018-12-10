#include "common/Mesh.hpp"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, Material & mat) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->mat.Ambient = mat.Ambient;
	this->mat.Diffuse = mat.Diffuse;
	this->mat.Specular = mat.Specular;
	this->mat.Shininess = mat.Shininess;

	/*
	std::cout << "Printing Mesh Material"<< std::endl;
	std::cout << "Ambient: R: " << mat.Ambient.x << " G: " << mat.Ambient.y << " B:" << mat.Ambient.z << std::endl;
	std::cout << "Diffuse: R: " << mat.Diffuse.x << " G: " << mat.Diffuse.y << " B:" << mat.Diffuse.z << std::endl;
	std::cout << "Specular: R: " << mat.Specular.x << " G: " << mat.Specular.y << " B:" << mat.Specular.z << std::endl;
	std::cout << "Shininess: " << mat.Shininess << std::endl;
	*/	
	setupMesh();
}

Mesh::~Mesh() {}

void Mesh::draw(Shader shader) {

	if(textures.size()) {
		shader.setBool("hasTexture", true);
		GLuint TextureUniform = glGetUniformLocation(shader.ID, "material.diffuse"); 
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(TextureUniform, 0);	
		glBindTexture(GL_TEXTURE_2D, textures[0].id);
	}
	else {
		shader.setBool("hasTexture", false);
		shader.setVec3("material.ambient", mat.Ambient);
		shader.setVec3("material.diffuse", mat.Diffuse);
		shader.setVec3("material.specular", mat.Specular);
		shader.setFloat("material.shininess", mat.Shininess);
	}

	// drawing without textures
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	/*
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// Vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	*/
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// Unbind VAO
	glBindVertexArray(0);

	/*
	for(GLuint i = 0; i < vertices.size(); i++) {
		std::cout << i << ":" << vertices[i].Normal.x << " "
							  << vertices[i].Normal.y << " "
							  << vertices[i].Normal.z
							  << std::endl;
	}
	*/
}
