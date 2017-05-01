#include "Camera.h"



Camera::Camera(glm::vec3 position) : position(position) {

}

Camera::~Camera() {

}

void Camera::update(float time_delta) {
}

bool Camera::move(glm::vec3 translationVec) {
	return false;
}



