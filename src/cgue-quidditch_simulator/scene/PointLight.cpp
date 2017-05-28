#include "PointLight.h";



PointLight::PointLight(glm::vec3 position) {
	this->pos = position;
}

PointLight::~PointLight() {


}

glm::vec3 PointLight::getPosition() {
	return this->pos;
}