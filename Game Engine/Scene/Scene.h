#pragma once
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include "SceneList.h"
#include "../Shader/Shader.h"
#include "../Model/Model.h"
#include "../Mesh/Mesh.h"
#include "../Camera/Camera.h"
#include "../Skybox/Skybox.h"
#include "../Luci/Shadows/Shadows.h"
#include "../Collision Detection/Octree/Octree.h"

class Scene {
public:
	Scene(Camera &camera);
	void loadScene(GLuint number, std::shared_ptr<Octree> Octree);
	void drawScene();
	void deleteScene();
	void updatePositions();
	SceneList getSL();

private:
	SceneList _SL;
	Model* _Models;
	Mesh* _Meshes;
	Shader* _Shaders;
	Skybox* _Skybox;
	Camera &_camera;
	Shadow _Shadow;
	//Octree* _Octree;
};