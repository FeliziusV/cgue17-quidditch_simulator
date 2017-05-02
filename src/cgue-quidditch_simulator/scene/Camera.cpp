#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtx/transform.hpp>


Camera::Camera(glm::vec3 position, glm::vec3 lookAt) : position(position), lookAt(lookAt) {
	view = glm::translate(glm::mat4(1), position);
	speed = glm::vec3(0.0f, 0.0f, -1.0f);

}

Camera::~Camera() {

}

void Camera::update(float time_delta) {
	this->time_delta = time_delta;
	view = view * glm::translate(glm::mat4(), speed * time_delta);

	view_projection = projection * view;
}

bool Camera::rise(glm::mat4 translation) {
	view = view * translation;
	return true;
}

bool Camera::rotate(glm::mat4 rotation) {
	view = view * rotation;
	return true;
}

bool Camera::accelerate() {
	if (speed.z >= -2.0f) {
		speed.z += -0.1f;
		return true;
	}
	return false;
}

bool Camera::decelerate() {
	if (speed.z > -1.0f) {
		speed.z += 0.1f;
		return true;
	}
	return false;
}

void Camera::project(int width, int height, float nearPlane, float farPlane) {
	view = glm::lookAt(position, lookAt, glm::vec3(0, 1, 0)); //last parameter = upVector, for looking upside down (0, -1, 0)
	projection = glm::perspective(60.0f, width / (float)height, nearPlane, farPlane);
	view_projection = projection * view;

}
