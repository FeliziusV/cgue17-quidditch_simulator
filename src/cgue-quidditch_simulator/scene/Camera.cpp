#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm\glm.hpp>


enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

Camera::Camera(glm::vec3 position) : FRONT(glm::vec3(0.0f, 0.0f, -1.0f)) {
	this->position = position;
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	
}

Camera::~Camera() {

}

void Camera::update(float time_delta) {
	/*this->time_delta = time_delta;
	view = view * glm::translate(glm::mat4(), speed * time_delta);

	view_projection = projection * view;
	*/
	this->position += FRONT * (speed.z * time_delta);
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

bool Camera::rise(float yoffset) {
	this->pitch += yoffset;
	return true;
}

bool Camera::rotate(float xoffset) {
	this->yaw += xoffset;
	return true;
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

void Camera::project(int width, int height, float nearPlane, float farPlane) {
	//view = glm::lookAt(position, lookAt, glm::vec3(0, 1, 0)); //last parameter = upVector, for looking upside down (0, -1, 0)
	projection = glm::perspective(60.0f, width / (float)height, nearPlane, farPlane);
	//view_projection = projection * view;

}
