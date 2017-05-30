#pragma once
#include <vector>
#include "glm\vec2.hpp"
#include "glm\vec3.hpp"
using namespace ::std;

class Mesh {

public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void draw(Shader shader);

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};