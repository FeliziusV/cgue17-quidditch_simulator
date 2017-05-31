#pragma once
#include <GL\glew.h>
#include <vector>
#include "glm\vec2.hpp"
#include "glm\vec3.hpp"
#include "../shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;
using namespace cgue;
class Mesh {
public:

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;

	};

	struct Texture {
		GLuint id;
		string type;
		aiString path;
	};

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void draw(Shader shader);

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};