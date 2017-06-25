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
	
	if(speed.z)
	this->position += FRONT * (speed.z * time_delta);
	this->mposition += mFRONT * (speed.z * time_delta);
	doMovement(time_delta);
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->FRONT = glm::normalize(front);
	this->RIGHT = glm::normalize(glm::cross(this->FRONT, this->worldUp));
	this->UP = glm::normalize(glm::cross(this->RIGHT, this->FRONT));

	glm::vec3 mfront;
	mfront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	mfront.y = sin(glm::radians(-this->pitch));
	mfront.z = sin(glm::radians(-this->yaw)) * cos(glm::radians(this->pitch));
	this->mFRONT = glm::normalize(mfront);
	this->mRIGHT = glm::normalize(glm::cross(this->mFRONT, this->worldUp));
	this->mUP = glm::normalize(glm::cross(this->mRIGHT, this->mFRONT));
	calcViewMatrix();
	
}

void Camera::calcViewMatrix() {
	this->view = glm::lookAt(this->position, this->position + this->FRONT, this->UP);
	this->model = glm::lookAt(this->position, this->position + this->mFRONT, this->mUP);
	//this->model = glm::translate(model, this->position);
	//this->model = glm::lookAt(this->position, this-> position + this->FRONT, this->UP);
}

void Camera::doMovement(float time_delta) {
	if (this->space) {
		if (pitch < 88.0f) {
			this->pitch += 100.0f * time_delta;
		}
	}
	if (this->shift) {
		if (pitch > -88.0f) {
			this->pitch += -100.0f * time_delta;
		}
	}
	if (this->left) {
		this->yaw += -100.0f * time_delta;
	}
	if (this->right) {
		this->yaw += 100.0f  * time_delta;
	}
	if (this->up) {
		if (speed.z <= 100.0f) {
			speed.z += 0.1f;
		}
	}
	if (this->down) {
		if (speed.z > -100.0f) {
			speed.z += -0.1f;
		}
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


