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
#include "../SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Shader/Shader.h"
#include "../Mesh/Mesh.h"

class Model {
public:
	Model(const GLchar *path);
	void Draw(Shader shader);

private:
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	GLint TextureFromFile(const char *path, std::string directory);
};