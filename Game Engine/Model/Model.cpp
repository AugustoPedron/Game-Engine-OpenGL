#include "Model.h"

Model::Model(std::vector<std::string> paths) {
	std::vector<std::string>::iterator itPaths = paths.begin();
	int i = 0;

	for (itPaths; itPaths != paths.end(); itPaths++, i++) {
		ModelLoader ML(paths[i].c_str());
		this->Models[paths[i].c_str()] = ML;
	}
}

void Model::Draw(ShaderLoader shaderLoader, std::vector<glm::mat4> modelsPositions, Camera &camera) {
	shaderLoader.use();
	shaderLoader.SetVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	shaderLoader.SetMat4("projection", camera.getProjection());
	shaderLoader.SetMat4("view", camera.GetViewMatrix());
	DirectionalLightSetter::SetDirectionalLight(shaderLoader);
	std::map<const GLchar*, ModelLoader>::iterator it = this->Models.begin();
	GLuint i = 0;
	for (it; it != this->Models.end(); it++, i++) {
		shaderLoader.SetMat4("model", modelsPositions[i]);
		it->second.Draw(shaderLoader);
	}
}

void Model::DeleteModels() {
	std::map<const GLchar*, ModelLoader>::iterator it = this->Models.begin();

	for (it; it != this->Models.end(); it++) {
		it->second.DeleteModel();
	}
}