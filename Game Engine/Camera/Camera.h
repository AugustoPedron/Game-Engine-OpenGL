#pragma once

#include <vector>
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
const float near_plane = 1.0f, far_plane = 150.0f;
const glm::vec3 lightPos(20.0f, 100.0f, 30.0f);

class Camera {
public:
	Camera() {};
	Camera(int screen_widht, int screen_height, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	void processKeyboard(Camera_movements direction, GLfloat deltaTime);
	void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(GLfloat yOffset);
	GLfloat getZoom();
	glm::vec3 getPosition();
	glm::mat4 GetViewMatrix();
	glm::vec3 getFront();
	glm::mat4 getProjection();
	inline int getScrWidth() { return this->screen_width; }
	inline int getScrHeight() { return this->screen_height; }
	inline glm::mat4 getLightSpaceMatrix() { return this->lightSpaceMatrix; }

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	glm::mat4 projection;
	glm::mat4 lightSpaceMatrix;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat mouseSens;
	GLfloat movementSpeed;
	GLfloat zoom;
	int screen_width;
	int screen_height;
	void updateCameraVectors();
};