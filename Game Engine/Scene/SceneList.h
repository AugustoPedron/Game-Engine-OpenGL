#pragma once
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Strutture/MeshStruct.h"

struct SceneComponents {
	std::vector<MeshStruct> Meshes;
	std::vector<std::string> Models;
	std::vector<std::string> Shaders;
	SkyboxStruct Skybox;
};

class SceneList {
private:
	SceneComponents SceneComp;
	
	std::vector<glm::mat4> ModelsPositions;

public :
	SceneList() {};
	SceneComponents getSceneComp();
	std::vector<glm::mat4> getMeshesPositions();
	std::vector<glm::mat4> getModelsPositions();
	std::vector<glm::mat4> MeshesPositions;
	void selectScene(GLuint number);
};