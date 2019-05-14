#include "Scene.h"

Scene::Scene(Camera &camera) : _camera(camera)
{
}

void Scene::loadScene(GLuint number)
{
	this->_SL.selectScene(number);
	this->_Models = Model(this->_SL.getSceneComp().Models);
	this->_Meshes = Mesh(this->_SL.getSceneComp().Meshes);
	this->_Shaders = Shader(this->_SL.getSceneComp().Shaders);
	this->_Skybox = Skybox(this->_SL.getSceneComp().Skybox);
}

void Scene::drawScene() {
	this->_Models.Draw(this->_Shaders.getShaders("modelLoading"), this->_SL.getModelsPositions(), _camera);
	this->_Meshes.Draw(this->_Shaders.getShaders("lighting"), this->_SL.getMeshesPositions(), _camera);
	this->_Skybox.Draw(this->_Shaders.getShaders("cubeMap"), _camera);
}

SceneList Scene::getSL() {
	return this->_SL;
}

void Scene::deleteScene() {
	this->_Models.DeleteModels();
	this->_Meshes.DeleteMeshes();
	this->_Skybox.DeleteSkybox();
}