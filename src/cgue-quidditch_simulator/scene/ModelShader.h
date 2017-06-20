#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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


};

