#pragma once

#include "GL\glew.h"
#include "glm\vec3.hpp"
#include <glm\gtc\matrix_transform.hpp>

class Camera {

public: 
	Camera(glm::vec3 position, glm::vec3 lookAt);
	~Camera();

	void update(float time_delta);
	void project(int width, int height, float nearPlane, float farPlane);
	bool rotate(glm::mat4 rotation);
	bool rise(glm::mat4 translation);
	bool accelerate();
	bool decelerate();

	glm::vec3 position;
	glm::vec3 lookAt;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 view_projection;
	glm::mat4 rotation;

	bool xPos;
	bool xNeg;
	bool yPos;
	bool yNeg;
	
private:
	glm::vec3 speed;
	float time_delta;



};