#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "../Collision Detection/Octree/Octree.h"

const GLfloat EPSILON = 0.0001F;

class Fisica {
private:

	glm::vec3 direction;
	glm::vec3 velocity;
	glm::vec3 mouseFactor;
	GLfloat MouseLeftPressTime;
	GLfloat MouseLeftReleaseTime;
	GLfloat deltaMouseTime;
	GLuint movingCondition;

	void adjustDirection();

public:

	Fisica();
	void moveObject(std::shared_ptr<Octree> Octree, GLfloat deltaTime, glm::vec3& cameraMmovement, glm::vec3 direction, bool& mouseEnabled);
	void changeDirection(GLuint dir);
	void setStartTime(GLfloat time);
	void setEndTime(GLfloat time);
};