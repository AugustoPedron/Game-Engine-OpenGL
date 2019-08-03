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
	Model(std::vector<ModelDefinition> models, std::shared_ptr<Octree> octree);
	void Draw(ShaderLoader* shaderLoader, Camera &camera, unsigned int shadowMap);
	void DeleteModels();
	inline std::vector<std::shared_ptr<ModelLoader>>& getModels() { return this->Models; }

private:
	//std::map<const GLchar*, ModelLoader> Models;
	std::vector<std::shared_ptr<ModelLoader>> Models;
};