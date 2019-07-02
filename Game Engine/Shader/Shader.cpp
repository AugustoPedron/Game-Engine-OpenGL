#include "Shader.h"

Shader::Shader(std::vector<std::string> shaders) {
	std::vector<std::string>::iterator it = shaders.begin();
	int i = 0;
	for (it; it != shaders.end(); it++, i++) {
		this->Shaders[shaders[i]] = (ShaderLoader(("../Game Engine/GLSL/" + shaders[i] + ".vs").c_str(), ("../Game Engine/GLSL/" + shaders[i] + ".frag").c_str()));
	}
}

ShaderLoader* Shader::getShaders(std::string shaderName) {
	return &this->Shaders.at(shaderName);
}