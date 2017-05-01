#pragma once

#include "GL\glew.h"
#include "glm\vec3.hpp"
#include <glm\gtc\matrix_transform.hpp>

class Camera {

public: 
	Camera(glm::vec3 position);
	~Camera();

	void update(float time_delta);
	bool move(glm::vec3 translationVec);
	bool move(glm::vec3 translationVec, glm::vec3 speed);

	glm::vec3 position;
	glm::mat4 view;
	
private:
	glm::vec3 speed;


};