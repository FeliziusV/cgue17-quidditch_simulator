#pragma once

#include <string>
#include <GL\glew.h>

namespace cgue
{
	class ModelTexture
	{
	public:
		ModelTexture(const char* path, std::string directory);
		~ModelTexture();

		void bind(int unit);
		GLuint loadTexture();

	private:
		int height;
		int width;
		std::string path;
		GLuint handle;

	};
}