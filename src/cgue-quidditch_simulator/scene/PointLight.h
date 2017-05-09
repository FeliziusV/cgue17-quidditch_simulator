#pragma once
#include "glm\vec3.hpp"


class PointLight {

public:
	PointLight(glm::vec3 position);

	~PointLight();

	glm::vec3 pos;


};