#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/types.h>
#include "../../Shader/ShaderLoader.h"
#include "../../Model/Model.h"
#include "../../Mesh/Mesh.h"

class Shadow {
private:
	const int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	unsigned int quadVAO;
	unsigned int quadVBO;

public:
	Shadow();
	void DrawShadows(ShaderLoader shadowShader, Mesh &Meshes, Model &Models,
	const std::vector<glm::mat4> &MeshesPositions, const std::vector<glm::mat4> &ModelsPositions, Camera &camera);
	void DrawShadowsDebug(ShaderLoader shadowShader);
	inline int getShadowWidth() { return SHADOW_WIDTH; }
	inline int getShadowHeight() { return SHADOW_HEIGHT; }
	inline unsigned int getDepthMap() { return this->depthMap; }
};