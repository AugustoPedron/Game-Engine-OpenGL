#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../SOIL2/SOIL2.h"
#include "../Mesh/MeshLoader.h"

class ModelLoader {
public:
	ModelLoader() {};
	ModelLoader(const GLchar *path);
	void Draw(ShaderLoader shader);
	void DeleteModel();

private:
	std::string directory;
	std::vector<MeshLoader> meshes;
	std::vector<structTexture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	MeshLoader processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<structTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char *path, std::string directory);
};