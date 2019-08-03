#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "../Collision Detection/Octree/Octree.h"

class Fisica {
public:
	static void moveObject(std::shared_ptr<Octree> Octree);
};