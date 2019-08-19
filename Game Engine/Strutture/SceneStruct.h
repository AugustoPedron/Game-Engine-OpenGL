#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MeshStruct.h"

struct ModelDefinition {
	std::string pathModel;
	std::string pathBoundingBox;
	std::string path;
	float angle;
	glm::mat4 position;
	glm::vec3 positionPhy;
};

struct MeshDefinition {
	glm::mat4 position;
	MeshStruct mesh;
};

struct SceneComponents {
	std::vector<MeshDefinition> Meshes;
	std::vector<ModelDefinition> Models;
	std::vector<std::string> Shaders;
	SkyboxStruct Skybox;
};