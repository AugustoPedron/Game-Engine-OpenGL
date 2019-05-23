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
	this->_Shadow.DrawShadows(this->_Shaders.getShaders("shadowShader"), this->_Meshes, this->_Models, this->_SL.getMeshesPositions(), this->_SL.getModelsPositions(),this->_camera);	
	glViewport(0, 0, _camera.getScrWidth(), _camera.getScrHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//this->_Shadow.DrawShadowsDebug(this->_Shaders.getShaders("shadowShaderDebug"));
	this->_Models.Draw(this->_Shaders.getShaders("modelLoading"), this->_SL.getModelsPositions(), _camera, this->_Shadow.getDepthMap());
	this->_Meshes.Draw(this->_Shaders.getShaders("lighting"), this->_SL.getMeshesPositions(), _camera, this->_Shadow.getDepthMap());
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