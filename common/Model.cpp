#include "common/Model.hpp"

Model::Model(char* path) {
	this->model = glm::mat4(1.0);
	loadModel(path);
}

Model::~Model() {}

void Model::draw(Shader shader) {
	shader.setMat4("model", this->model);
	for(GLuint i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);		
	}
}

void Model::setModelMatrix(glm::mat4 & model) {
	this->model = model;
}

void Model::loadModel(char* path) {
	Assimp::Importer importer;
	//const aiScene *scene = importer.ReadFile(path, 	aiProcess_Triangulate | aiProcess_FlipUVs);
	const aiScene *scene = importer.ReadFile(path, 	aiProcess_Triangulate | aiProcess_JoinIdenticalVertices );

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	string str = path;
	directory =str.substr(0, str.find_last_of('/'));

	processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode *node, const aiScene *scene) {
	// Process all the node's meshes (if any)
	for(GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// Do the same for each of its children
	for(GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene * scene) {
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	for(GLuint i = 0; i < mesh->mNumVertices; i++) {

		Vertex vertex;
		// Process vertex positions, normals and textures
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;

		vertex.Normal = vector;
		
		if(mesh->mTextureCoords[0]) {// Does the mesh contain texture coordinates? 
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);

	}
	
	// Process indices
	for(GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process material
	if(mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		// 1. Diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material,
											aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material,
											aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		//aiColor3D color(0.0f, 0.0f, 0.0f);
		aiColor3D amb(0.0f, 0.0f, 0.0f);
		aiColor3D dif(0.0f, 0.0f, 0.0f);
		aiColor3D spe(0.0f, 0.0f, 0.0f);
		float shine;
		material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, dif);
		material->Get(AI_MATKEY_COLOR_SPECULAR, spe);
		material->Get(AI_MATKEY_SHININESS, shine);

		std::cout << "Printing Mesh Material"<< std::endl;
		std::cout << "Ambient: R: " << amb.r << " G: " << amb.g << " B:" << amb.b << std::endl;
		std::cout << "Diffuse: R: " << dif.r << " G: " << dif.g << " B:" << dif.b << std::endl;
		std::cout << "Specular: R: " << spe.r << " G: " << spe.g << " B:" << spe.b << std::endl;
		std::cout << "Shininess: " << shine << std::endl;
		
	}


	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
	

	vector<Texture> textures;
	for(GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}

	return textures;
}


/*
	Loads a texture image from a file. Using own texture loader function with DevIL
	instead of learnopengl tutorial.	
*/
// TODO: Check if it can find the file
GLuint TextureFromFile(const char *path, const string &directory, bool gamma) {
	string filename = string(path);
	filename = directory + '/' + filename;
	//std::cout << "Loading texture from file...\n";
	//std::cout << "Filename: " << filename << std::endl; 
	return loadTexture(filename.c_str());
}
