#include "Camera.h"

Camera::Camera(int screen_width, int screen_height, GLfloat position, bool* cameraMode, glm::vec3 up, GLfloat yaw, GLfloat pitch)
	: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSens(SENS), zoom(ZOOM) {
	this->distanca = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->screen_height = screen_height;
	this->screen_width = screen_width;
	this->objectPosition = NULL;
	this->cameraMode = cameraMode;
	this->projection = glm::perspective(this->zoom, (GLfloat)this->screen_width / (GLfloat)this->screen_height, 0.1f, 1000.0f);
	glm::mat4 lightView, lightProjection;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	this->lightSpaceMatrix = lightProjection * lightView;
	this->updateCameraVectors();
}


Camera::Camera(GLfloat position, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch, bool* cameraMode)
	: front(glm::vec3(0.0f, -0.6f, -1.0f)), movementSpeed(SPEED), mouseSens(SENS), zoom(ZOOM) {
	this->distanca = position;
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	this->objectPosition = NULL;
	this->cameraMode = cameraMode;
	this->updateCameraVectors();
}

void Camera::processKeyboard(Camera_movements direction, GLfloat deltaTime) {
	GLfloat velocity = this->movementSpeed * deltaTime;
	if (FORWARD == direction) {
		this->position += this->front * velocity;
	}
	if (BACKWARD == direction) {
		this->position -= this->front * velocity;
	}
	if (LEFT == direction) {
		this->position -= this->right * velocity;
	}
	if (RIGHT == direction) {
		this->position += this->right * velocity;
	}
}

void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch) {
	xOffset *= this->mouseSens;
	yOffset *= this->mouseSens;
	this->yaw += xOffset;
	this->pitch += yOffset;
	if (constrainPitch) {
		if (this->pitch > 89.0f) {
			this->pitch = 89.0f;
		}
		if (this->pitch < -89.0f) {
			this->pitch = -89.0f;
		}
	}

	this->updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yOffset) {
	/*if (this->zoom >= 1.0f && this->zoom <= 45.0f) {
		this->zoom -= yOffset / 10;
	}

	if (this->zoom < 1.0f) {
		this->zoom = 1.0f;
	}
	if (this->zoom > 45.0f) {
		this->zoom = 45.0f;
	}*/
}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
	if (*cameraMode) this->updatePosition();
}

void Camera::movement(glm::vec3 movement) {
	this->position += movement;
}

GLfloat Camera::getZoom() {
	return this->zoom;
}

glm::vec3 Camera::getPosition() {
	return this->position;
}

glm::vec3 Camera::getFront() {
	return this->front;
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(this->position, this->position + this->front - glm::vec3(0.0f, 0.15f, 0.0f), this->up);
}

glm::mat4 Camera::getProjection() {
	return this->projection;
}

void Camera::updatePosition() {
	if (this->objectPosition != NULL) {
		glm::vec3 relativePosition = -this->front * this->distanca + glm::vec3(0.0f, 1.5f, 0.0f);
		this->position = relativePosition + *this->objectPosition;
	}
}

void Camera::setObjectPosition(glm::vec3* position) {
	this->objectPosition = position;
	this->updatePosition();
}