#pragma once

#include "ModelLoader.h"
#include "../Camera/Camera.h"
#include "../Luci/Luci/Spot_Light.h"
#include "../Luci/Luci/Directional_Light.h"
#include "../Luci/Luci/Point_Light.h"
#include "../Collision Detection/Octree/Octree.h"
#include "../Strutture/SceneStruct.h"

class Model {
public:
	Model() {};
	Model(std::vector<ModelDefinition> models, Octree *octree);
	void Draw(ShaderLoader* shaderLoader, Camera &camera, unsigned int shadowMap);
	void DeleteModels();
	inline std::map<const GLchar*, ModelLoader>& getModels() { return this->Models; }

private:
	std::map<const GLchar*, ModelLoader> Models;
};