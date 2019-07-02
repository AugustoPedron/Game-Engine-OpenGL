#include "Model.h"

Model::Model(std::vector<ModelDefinition> models, Octree *octree) {
	std::vector<ModelDefinition>::iterator itPaths = models.begin();
	int i = 0;

	for (itPaths; itPaths != models.end(); itPaths++, i++) {
		ModelLoader ML(models[i], i);
		octree->addObject(ML);
		this->Models[models[i].path.c_str()] = ML;
	}
}

void Model::Draw(ShaderLoader* shaderLoader, Camera &camera, unsigned int shadowMap) {
	shaderLoader->use();
	shaderLoader->SetVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	shaderLoader->SetMat4("projection", camera.getProjection());
	shaderLoader->SetMat4("view", camera.GetViewMatrix());
	DirectionalLightSetter::SetDirectionalLight(shaderLoader);
	std::map<const GLchar*, ModelLoader>::iterator it = this->Models.begin();
	GLuint i = 0;
	for (it; it != this->Models.end(); it++, i++) {
		it->second.Draw(shaderLoader, shadowMap);
	}
}

void Model::DeleteModels() {
	std::map<const GLchar*, ModelLoader>::iterator it = this->Models.begin();

	for (it; it != this->Models.end(); it++) {
		it->second.DeleteModel();
	}
	std::map<const GLchar*, ModelLoader>().swap(this->Models);
}