#pragma once
#include <GL\glew.h>
#include <vector>
#include "glm\vec2.hpp"
#include "glm\vec3.hpp"
#include "ModelShader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;
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
		aiString path;	// we store the path of the texture to compare with other textures
	};

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void draw(ModelShader shader);

private:
	GLuint VAO, VBO, EBO;

	void setupMesh();
};