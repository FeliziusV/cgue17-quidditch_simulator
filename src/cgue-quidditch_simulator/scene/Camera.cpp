#include "Camera.h"
#include <glm\gtc\type_ptr.hpp>


Camera::Camera(glm::vec3 position) : position(position) {
	view = glm::translate(glm::mat4(1), position);
}

Camera::~Camera() {

}

void Camera::update(float time_delta) {
	//Camera::move(time_delta * glm::vec3(0.5f), glm::vec3(0.0f));
}

bool Camera::move(glm::vec3 translationVec, glm::vec3 speed) {
	view = glm::translate(view, translationVec);

	return true;
}

bool Camera::move(glm::vec3 translationVec) {
	view = glm::translate(view, translationVec);

	return true;
}


