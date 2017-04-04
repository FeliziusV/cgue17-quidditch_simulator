#pragma once
#include <GL\glew.h>
#include "SceneObject.h"
#include "shader1.h"
#ifdef SCENE_OBECT

#define CUBE_VERTEXT_COUNT 8
#define CUBE_INDEX_COUNT 36

namespace cgue {
	namespace scene {
		class Cube : public SceneObject {

		public:
			Cube();
			Cube(glm::mat4& matrix, Shader* shader);
			virtual ~Cube();

			virtual void update();
			virtual void draw();
		private:
			GLuint vao;
			GLuint positionBuffer, indexBuffer;
			Shader* shader;

			static vonst float positions[CUBE_VERTEX_COUNT * 3];
			static const unsigned int indices[CUBE_INDEX_COUNT];
		};
	}
}


#endif