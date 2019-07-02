#include "Scene.h"

Scene::Scene(Camera &camera) : _camera(camera)
{
}

void Scene::loadScene(GLuint number)
{
	this->_SL.selectScene(number);
	OctreeBounds bounds;
	bounds.position = glm::vec3(0.0f, -1.75f, 0.0f);
	bounds.size = 20.0f;
	this->_Octree = new Octree(NULL, bounds, 5, 0);
	this->_Models = new Model(this->_SL.getSceneComp().Models, this->_Octree);
	this->_Meshes = new Mesh(this->_SL.getSceneComp().Meshes);
	this->_Shaders = new Shader(this->_SL.getSceneComp().Shaders);
	this->_Skybox = new Skybox(this->_SL.getSceneComp().Skybox);
}

void Scene::drawScene() {
	this->_Shadow.DrawShadows(this->_Shaders->getShaders("shadowShader"), this->_Meshes, this->_Models, this->_camera);
	glViewport(0, 0, _camera.getScrWidth(), _camera.getScrHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//this->_Shadow.DrawShadowsDebug(this->_Shaders.getShaders("shadowShaderDebug"));
	this->_Models->Draw(this->_Shaders->getShaders("modelLoading"), _camera, this->_Shadow.getDepthMap());
	this->_Meshes->Draw(this->_Shaders->getShaders("lighting"), _camera, this->_Shadow.getDepthMap());
	this->_Skybox->Draw(this->_Shaders->getShaders("cubeMap"), _camera);
}

void updatePositions() {

}

SceneList Scene::getSL() {
	return this->_SL;
}

void Scene::deleteScene() {
	this->_Models->DeleteModels();
	this->_Meshes->DeleteMeshes();
	this->_Skybox->DeleteSkybox();
}