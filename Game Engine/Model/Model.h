#pragma once

#include "ModelLoader.h"
#include "../Camera/Camera.h"
#include "../Fisica/Luci/Spot_Light.h"
#include "../Fisica/Luci/Directional_Light.h"
#include "../Fisica/Luci/Point_Light.h"

class Model {
public:
	Model() {};
	Model(std::vector<std::string> paths);
	void Draw(ShaderLoader shaderLoader, std::vector<glm::mat4> modelsPositions, Camera &camera, unsigned int shadowMap);
	void DeleteModels();
	inline std::map<const GLchar*, ModelLoader>& getModels() { return this->Models; }

private:
	std::map<const GLchar*, ModelLoader> Models;
};