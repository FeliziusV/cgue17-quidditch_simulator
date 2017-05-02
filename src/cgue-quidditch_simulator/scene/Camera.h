#pragma once

#include "GL\glew.h"
#include "glm\vec3.hpp"
#include <glm\gtc\matrix_transform.hpp>

class Camera {

public: 
	Camera(glm::vec3 position);
	~Camera();

	void update(float time_delta);
	void project(int width, int height, float nearPlane, float farPlane);
	void calcViewMatrix();
	bool rotate(float xoffset);
	bool rise(float yoffset);
	bool accelerate();
	bool decelerate();
	
	glm::vec3 lookAt;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 view_projection;
	glm::mat4 rotation;

	float yaw;
	float pitch;
	glm::vec3 position;
	glm::vec3 FRONT;
	glm::vec3 RIGHT;
	glm::vec3 UP;
	glm::vec3 worldUp;

	bool xPos;
	bool xNeg;
	bool yPos;
	bool yNeg;
	
private:
	glm::vec3 speed;
	float time_delta;



};