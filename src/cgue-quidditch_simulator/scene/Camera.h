#pragma once

#include "GL\glew.h"
#include "glm\vec3.hpp"

class Camera {

public: 
	Camera(glm::vec3 position);
	~Camera();

	void update(float time_delta);
	bool move(glm::vec3 translationVec);

	glm::vec3 position;
	
private:
	glm::vec3 speed;


};