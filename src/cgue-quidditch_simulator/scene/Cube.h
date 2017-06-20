#pragma once
#include "SceneObject.h"
#include "..\shader.h"


#define CUBE_VERTEX_COUNT 8
#define CUBE_INDEX_COUNT 36

namespace cgue {
	namespace scene {
		class Cube : public SceneObject {

		public:
			Cube();
			Cube(glm::mat4& matrix, Shader* shader);
			virtual ~Cube();

			virtual void update(float time_delta);
			virtual void draw();
		private:
			GLuint vao;
			GLuint positionBuffer, normalsBuffer, indexBuffer, uvBuffer;
			Shader* shader;

			static const float positions[CUBE_VERTEX_COUNT * 3];
			static const unsigned int indices[CUBE_INDEX_COUNT];
			static const float normals[CUBE_VERTEX_COUNT * 9];
			static const unsigned int new_indices[CUBE_INDEX_COUNT];
			static const float new_positions[CUBE_VERTEX_COUNT * 9];
			static const float uvs[CUBE_VERTEX_COUNT * 6];
			float counter = 0.0f;

		};
	}
}

