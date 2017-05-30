#pragma once
#include <GL\glew.h>
#include <vector>
#include "glm\vec2.hpp"
#include "glm\vec3.hpp"
#include "../shader.h"
using namespace std;
using namespace cgue;
class Mesh {

public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void draw(Shader shader);

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};