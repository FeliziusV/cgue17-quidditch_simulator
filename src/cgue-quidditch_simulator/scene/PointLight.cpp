#include "PointLight.h";
#include <glm/gtc/matrix_transform.hpp>;



PointLight::PointLight(glm::vec3 position) {
	this->pos = position;
}

PointLight::~PointLight() {


}

glm::vec3 PointLight::getPosition() {
	return this->pos;
}


void PointLight::move(float time_delta) {
	if (counter >= 0.0f) {
		this->pos = this->pos + glm::vec3(4.0f * time_delta, 0.0f, 0.0f);
		counter = counter + 1.0f * time_delta;

	}
	if (counter < 0.0f) {
		this->pos = this->pos + glm::vec3(-4.0f * time_delta, 0.0f, 0.0f);
		counter = counter + 1.0f * time_delta;
	}

	if (counter >= 1.0f) {
		counter = -1.0f;
	}
}