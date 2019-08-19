#include "Fisica.h"

Fisica::Fisica() {
	this->direction = glm::vec3(0.0f, 0.0f, -1.0f);
	this->mouseFactor = glm::vec3(7.0f, 7.0f, 7.0f);
	this->MouseLeftPressTime = -1;
	this->MouseLeftReleaseTime = -1;
	this->deltaMouseTime = 1.0f;
}

void Fisica::moveObject(std::shared_ptr<Octree> Octree, GLfloat deltaTime, glm::vec3& cameraMmovement, glm::vec3 direction, bool& mouseEnabled) {
	if (this->velocity.x == 0 && this->velocity.y == 0 && this->velocity.z == 0) {
		this->direction = glm::vec3(direction);
		this->direction.y = 0;
		this->direction = glm::normalize(this->direction);
	}
	//deltaTime = 0.009f;
	GLfloat deltaTimeSec = deltaTime * 6.5;

	deltaTime /= 3.0f;

	bool flagX = true, flagY = true, flagZ = true;
	if (this->MouseLeftReleaseTime == -1) {
		mouseEnabled = true;
		return;
	}
	std::shared_ptr<ModelLoader> ML = Octree->getML(0);
	glm::vec3 movement = this->direction * this->velocity;
	if (this->direction.x > 0) {
		if (ML->getPositionPhy().x + 1.0f + (this->velocity.x * deltaTime * this->deltaMouseTime) < (Octree->getBounds().position.x + Octree->getBounds().size / 2)) {
			movement.x = movement.x * deltaTime * this->deltaMouseTime;
		}
		else {
			flagX = false;
		}
	}
	else if (this->direction.x < 0) {
		if (ML->getPositionPhy().x - 1.0f - (this->velocity.x * deltaTime * this->deltaMouseTime) > (Octree->getBounds().position.x - Octree->getBounds().size / 2)) {
			movement.x = movement.x * deltaTime * this->deltaMouseTime;
		}
		else {
			flagX = false;
		}
	}

	if (this->direction.y > 0) {
		if (ML->getPositionPhy().y + 1.0f + (this->velocity.y * deltaTime * this->deltaMouseTime) < (Octree->getBounds().position.y + Octree->getBounds().size / 2)) {
			movement.y = movement.y * deltaTime * this->deltaMouseTime;
		}
		else {
			flagY = false;
		}
	}
	else if (this->direction.y < 0) {
		if (ML->getPositionPhy().y - 1.0f - (this->velocity.y * deltaTime * this->deltaMouseTime) > (Octree->getBounds().position.y - Octree->getBounds().size / 2)) {
			movement.y = movement.y * deltaTime * this->deltaMouseTime;
		}
		else {
			flagY = false;
		}
	}

	if (this->direction.z > 0) {
		if (ML->getPositionPhy().z + 1.0f + (this->velocity.z * deltaTime * this->deltaMouseTime) < (Octree->getBounds().position.z + Octree->getBounds().size / 2)) {
			movement.z = movement.z * deltaTime * this->deltaMouseTime;
		}
		else {
			flagZ = false;
		}
	}
	else if (this->direction.z < 0) {
		if (ML->getPositionPhy().z - 1.0f - (this->velocity.z * deltaTime * this->deltaMouseTime) > (Octree->getBounds().position.z - Octree->getBounds().size / 2)) {
			movement.z = movement.z * deltaTime * this->deltaMouseTime;
		}
		else {
			flagZ = false;
		}
	}

	this->velocity += -4.0f * deltaTimeSec * deltaTimeSec;
	if (this->velocity.y < 0) this->velocity.y = 0;
	if (this->velocity.x < 0) this->velocity.x = 0;
	if (this->velocity.z < 0) this->velocity.z = 0;

	if (this->velocity.x == 0 && this->velocity.y == 0 && this->velocity.z == 0) {
		this->MouseLeftPressTime = -1;
		this->MouseLeftReleaseTime = -1;
	}

	//std::cout << ML->getPositionPhy().z + 1.0f + (this->velocity.z * deltaTime * this->deltaMouseTime) << std::endl;

	if (flagX && flagY && flagZ) {
		ML->updatePosition(movement);
		cameraMmovement = movement;
	}
	else {
		this->velocity = glm::vec3(0.0f);
	}

	//if (ML->getMovingCondition()) {
		Octree->updateOctree(0);
		Octree->checkCollisionRecursive(0, this->direction);
		this->direction = glm::normalize(this->direction);
	//}

	mouseEnabled = !ML->getMovingCondition();
}

void Fisica::changeDirection(GLuint dir) {
	if (dir == 1) {
		this->direction.x *= -1;
	}

	if (dir == 2) {
		this->direction.y *= -1;
	}

	if (dir == 3) {
		this->direction.z *= -1;
	}
}

void Fisica::setStartTime(GLfloat time) {
	if (this->MouseLeftPressTime == -1)
		this->MouseLeftPressTime = time;
}

void Fisica::setEndTime(GLfloat time) {
	if (this->MouseLeftReleaseTime == -1) {
		this->MouseLeftReleaseTime = time;
		this->deltaMouseTime = this->MouseLeftReleaseTime - this->MouseLeftPressTime;
		this->velocity = this->mouseFactor * this->deltaMouseTime;
	}
}

void Fisica::adjustDirection() {
	this->direction.x /= abs(this->direction.x);
	this->direction.y /= abs(this->direction.y);
	this->direction.z /= abs(this->direction.z);
}