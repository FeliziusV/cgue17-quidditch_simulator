#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "GL\glew.h"
using namespace std;

class ModelShader {
public:
	GLuint ID;
	ModelShader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();

	void setBool(const string &name, bool value) const;
	void setInt(const string &name, int value) const;
	void setFloat(const string &name, float value) const;
	void setVec3(const string &name, glm::vec3 value) const;
	void setMat4(const string &name, glm::mat4 value) const;


};

