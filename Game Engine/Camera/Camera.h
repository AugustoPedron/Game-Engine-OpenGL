#pragma once

#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_movements {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENS = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	void processKeyboard(Camera_movements direction, GLfloat deltaTime);
	void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(GLfloat yOffset);
	GLfloat getZoom();
	glm::vec3 getPosition();
	glm::mat4 GetViewMatrix();
	glm::vec3 getFront();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat mouseSens;
	GLfloat movementSpeed;
	GLfloat zoom;
	void updateCameraVectors();
};