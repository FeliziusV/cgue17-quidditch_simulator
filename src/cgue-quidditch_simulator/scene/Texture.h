#pragma once

#include <string>
#include <GL\glew.h>

namespace cgue
{
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();

		void bind(int unit);
		const void* loadTexture(std::string filenameString, unsigned int &width, unsigned int &height);

	private:
		GLuint handle;

	};
}