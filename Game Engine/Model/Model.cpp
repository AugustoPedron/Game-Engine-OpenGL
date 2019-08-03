#include "Model.h"

Model::Model(std::vector<ModelDefinition> models, std::shared_ptr<Octree> octree) {
	std::vector<ModelDefinition>::iterator itPaths = models.begin();
	int i = 0;

	for (itPaths; itPaths != models.end(); itPaths++, i++) {
		//ModelLoader ML(models[i], i);
		this->Models.push_back(std::make_shared<ModelLoader>(models[i], i));
		octree->addObject(this->Models[i]);
	}
}

void Model::Draw(ShaderLoader* shaderLoader, Camera &camera, unsigned int shadowMap) {
	shaderLoader->use();
	shaderLoader->SetVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	shaderLoader->SetMat4("projection", camera.getProjection());
	shaderLoader->SetMat4("view", camera.GetViewMatrix());
	DirectionalLightSetter::SetDirectionalLight(shaderLoader);
	std::vector<std::shared_ptr<ModelLoader>>::iterator it = this->Models.begin();
	GLuint i = 0;
	for (it; it != this->Models.end(); it++, i++) {
		(*it)->Draw(shaderLoader, shadowMap);
	}
}

void Model::DeleteModels() {
	std::vector<std::shared_ptr<ModelLoader>>::iterator it = this->Models.begin();

	for (it; it != this->Models.end(); it++) {
		it->reset();
	}
	std::vector<std::shared_ptr<ModelLoader>>().swap(this->Models);
}