#pragma once
#include <map>
#include "ShaderLoader.h"
#include "../Scene/SceneList.h"

class Shader {
private:
	std::map<std::string, ShaderLoader> Shaders;

public:
	Shader() {};
	Shader(std::vector<std::string> shaders);
	ShaderLoader getShaders(std::string shaderName);
};