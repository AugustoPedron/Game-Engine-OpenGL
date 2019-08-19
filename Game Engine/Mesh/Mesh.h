#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MeshLoader.h"
#include "../Texture/Texture.h"
#include "../Camera/Camera.h"
#include "../Luci/Luci/Spot_Light.h"
#include "../Luci/Luci/Directional_Light.h"
#include "../Luci/Luci/Point_Light.h"
#include "../Strutture/SceneStruct.h"

class Mesh {
public:
	Mesh() {};
	Mesh(std::vector<MeshDefinition> Meshes);
	~Mesh();
	void Draw(ShaderLoader* shaderLoader, Camera &camera, unsigned int shadowMap);
	inline std::vector<MeshLoader>& getMeshes() { return this->Meshes; }

private:
	std::vector<MeshLoader> Meshes;
};