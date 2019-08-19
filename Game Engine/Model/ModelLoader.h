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
#include <glm/gtx/matrix_decompose.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../SOIL2/SOIL2.h"
#include "../Mesh/MeshLoader.h"
#include "../Strutture/SceneStruct.h"
#include "../Strutture/MeshStruct.h"

class ModelLoader {
public:
	ModelLoader() {};
	ModelLoader(ModelDefinition model, GLuint id);
	~ModelLoader();
	void Draw(ShaderLoader* shader, unsigned int shadowMap);
	inline std::vector<std::shared_ptr<MeshLoader>>& getMeshes() { return this->meshes; }
	inline glm::mat4& getPosition() { return this->position; }
	inline glm::vec3& getPositionPhy() { return this->positionPhy; }
	inline glm::vec3& getLastTranslation() { return this->lastTranslation; }
	void updatePosition(glm::vec3 newPos);
	inline GLuint getId() { return this->id; }
	inline GLuint getMovingCondition() { return this->moving; }
	inline GLuint getStartMovingCondition() { return this->startMoving; }
	inline float getRotation() { return this->rotation; }
	void revertLastTranslation();
	void addModelPointer(std::shared_ptr<ModelLoader> pointer);

private:
	GLuint id;
	glm::mat4 position;
	glm::vec3 positionPhy;
	glm::vec3 lastTranslation;
	float rotation;
	GLuint startMoving, moving;
	std::string directory;
	std::vector<std::shared_ptr<MeshLoader>> meshes;
	std::vector<structTexture> textures_loaded;
	std::string path;
	std::shared_ptr<ModelLoader> model;
	void processNode(aiNode *node, const aiScene *scene);
	MeshLoader* processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<structTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char *path, std::string directory);
	void loadModel();
};