#include "Mesh.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};

struct Texture {
	GLuint id;
	string type;
};

