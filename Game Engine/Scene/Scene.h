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
#include "../Fisica/Shadows/Shadows.h"

class Scene {
public:
	Scene(Camera &camera);
	void loadScene(GLuint number);
	void drawScene();
	void deleteScene();
	SceneList getSL();

private:
	SceneList _SL;
	Model _Models;
	Mesh _Meshes;
	Shader _Shaders;
	Skybox _Skybox;
	Camera &_camera;
	Shadow _Shadow;
};