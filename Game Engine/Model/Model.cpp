#include "Model.h"

Model::Model(std::vector<ModelDefinition> models, std::shared_ptr<Octree> octree) {
	std::vector<ModelDefinition>::iterator itPaths = models.begin();
	int i = 0;

	for (itPaths; itPaths != models.end(); itPaths++, i++) {
		models[i].path=models[i].pathBoundingBox;
		this->Models.push_back(std::make_shared<ModelLoader>(models[i], i));
		octree->addObject(this->Models[i * 2]);
		models[i].path = models[i].pathModel;
		this->Models.push_back(std::make_shared<ModelLoader>(models[i], i));
		this->Models[i * 2]->addModelPointer(this->Models[i * 2 + 1]);
		this->Models[i * 2 + 1]->addModelPointer(NULL);
	}
}

void Model::Draw(ShaderLoader* shaderLoader, Camera &camera, unsigned int shadowMap) {
	shaderLoader->use();
	shaderLoader->SetVec3("viewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	shaderLoader->SetMat4("projection", camera.getProjection());
	shaderLoader->SetMat4("view", camera.GetViewMatrix());
	DirectionalLightSetter::SetDirectionalLight(shaderLoader);
	GLuint i = 0;
	for (i = 1; i <= this->Models.size(); i += 2) {
		this->Models[i]->Draw(shaderLoader, shadowMap);
	}
}

void Model::DeleteModels() {
	std::vector<std::shared_ptr<ModelLoader>>::iterator it = this->Models.begin();

	for (it; it != this->Models.end(); it++) {
		it->reset();
	}
	std::vector<std::shared_ptr<ModelLoader>>().swap(this->Models);
}

Model::~Model() {
	for (GLuint i = 0; i < this->Models.size(); i++) {
		this->Models[i].reset();
	}
	std::vector<std::shared_ptr<ModelLoader>>().swap(this->Models);
}