#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm\glm.hpp>
#include <iostream>


Camera::Camera(glm::vec3 position) : FRONT(glm::vec3(0.0f, 0.0f, -1.0f)) {
	this->position = position;
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	
	this->space = false;
	this->shift = false;
	this->up = false;
	this->down = false;
	this->left = false;
	this->right = false;
}

Camera::~Camera() {

}

void Camera::update(float time_delta) {
	
	this->position += FRONT * (speed.z * time_delta);
	doMovement(time_delta);
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->FRONT = glm::normalize(front);
	this->RIGHT = glm::normalize(glm::cross(this->FRONT, this->worldUp));
	this->UP = glm::normalize(glm::cross(this->RIGHT, this->FRONT));
	calcViewMatrix();
	
}

void Camera::calcViewMatrix() {
	this->view = glm::lookAt(this->position, this->position + this->FRONT, this->UP);
}

void Camera::doMovement(float time_delta) {
	if (this->space) {
		if (pitch < 89.0f) {
			this->pitch += 1.0f;
		}
	}
	if (this->shift) {
		if (pitch > -89.0f) {
			this->pitch += -1.0f;
		}
	}
	if (this->up) {
		if (speed.z <= 10.0f) {
			speed.z += 0.1f;
		}
	}
	if (this->down) {
		if (speed.z > 0.1f) {
			speed.z += -0.1f;
		}
	}
	if (this->left) {
		this->yaw += -1;

	}
	if (this->right) {
		this->yaw += 1;
	}

}


bool Camera::accelerate() {
	if (speed.z <= 5.0f) {
		speed.z += 0.1f;
		return true;
	}
	return false;
}

bool Camera::decelerate() {
	if (speed.z > 0.1f) {
		speed.z += -0.1f;
		return true;
	}
	return false;
}


