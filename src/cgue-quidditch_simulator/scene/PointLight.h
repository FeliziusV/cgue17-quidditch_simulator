#pragma once
#include "glm\vec3.hpp"


class PointLight {

public:
	PointLight(glm::vec3 position);

	~PointLight();

	glm::vec3 pos;

	glm::vec3 getPosition();

	void move(float time_delta);

	float ambient;
	float counter = 0.0f;


};