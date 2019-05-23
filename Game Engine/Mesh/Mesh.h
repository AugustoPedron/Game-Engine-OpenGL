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
#include "../Fisica/Luci/Spot_Light.h"
#include "../Fisica/Luci/Directional_Light.h"
#include "../Fisica/Luci/Point_Light.h"

struct MeshStruct;

class Mesh {
public:
	Mesh() {};
	Mesh(std::vector<MeshStruct> Meshes);
	void Draw(ShaderLoader shaderLoader, std::vector<glm::mat4> meshesPositions, Camera &camera, unsigned int shadowMap);
	void DeleteMeshes();
	std::vector<MeshLoader>& getMeshes() { return this->Meshes; }

private:
	std::vector<MeshLoader> Meshes;
};