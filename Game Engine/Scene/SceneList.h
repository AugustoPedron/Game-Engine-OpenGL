#pragma once
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "../Strutture/MeshStruct.h"
#include "../Strutture/SceneStruct.h"

class SceneList {
private:
	SceneComponents SceneComp;

public :
	SceneList() {};
	SceneComponents getSceneComp();
	void selectScene(GLuint number);
};