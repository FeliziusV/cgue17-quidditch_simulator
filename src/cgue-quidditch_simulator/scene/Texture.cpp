#include "Texture.h"
#include <FreeImage\FreeImage.h>
#include <iostream>

using namespace cgue;

Texture::Texture(const std::string& path) {

	//handle auf textur erstellen
	glGenTextures(1, &handle);
	//aktiviere die textureunit 0, da nur eine textur verwendet wird, später andere unit verwenden
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handle);

	unsigned int width, height;

	//load Texture
	auto textureData = loadTexture(path, width, height);

	// Construct the texture.
	// Note: The 'Data format' is the format of the image data as provided by the image library. FreeImage decodes images into
	// BGR/BGRA format, but we want to work with it in the more common RGBA format, so we specify the 'Internal format' as such.
	glTexImage2D(GL_TEXTURE_2D,    // Type of texture
		0,                // Mipmap level (0 being the top level i.e. full size)
		GL_RGBA,          // Internal format
		width,       // Width of the texture
		height,      // Height of the texture,
		0,                // Border in pixels
		GL_BGRA,          // Data format
		GL_UNSIGNED_BYTE, // Type of texture data
		textureData);     // The image data to use for this texture

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


Texture::~Texture() {
	glDeleteTextures(1, &handle);
}

void Texture::bind(int unit) {

	//unit = verwendete Texture unit siehe texture slides tag 4
	glActiveTexture(GL_TEXTURE0 + unit);
	//bind Texture
	glBindTexture(GL_TEXTURE_2D, handle);
}
//found on https://r3dux.org/2014/10/how-to-load-an-opengl-texture-using-the-freeimage-library-or-freeimageplus-technically/

const void* Texture::loadTexture(std::string filenameString, unsigned int &width, unsigned int &height) {

	// Get the filename as a pointer to a const char array to play nice with FreeImage
	const char* filename = filenameString.c_str();

	// Determine the format of the image.
	// Note: The second paramter ('size') is currently unused, and we should use 0 for it.
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	// Image not found? Abort! Without this section we get a 0 by 0 image with 0 bits-per-pixel but we don't abort, which
	// you might find preferable to dumping the user back to the desktop.
	if (format == -1)
	{
		std::cout << "Could not find image: " << filenameString << " - Aborting." << std::endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	// Found image, but couldn't determine the file format? Try again...
	if (format == FIF_UNKNOWN)
	{
		std::cout << "Couldn't determine file format - attempting to get from file extension..." << std::endl;

		// ...by getting the filetype from the filename extension (i.e. .PNG, .GIF etc.)
		// Note: This is slower and more error-prone that getting it from the file itself,
		// also, we can't use the 'U' (unicode) variant of this method as that's Windows only.
		format = FreeImage_GetFIFFromFilename(filename);

		// Check that the plugin has reading capabilities for this format (if it's FIF_UNKNOWN,
		// for example, then it won't have) - if we can't read the file, then we bail out =(
		if (!FreeImage_FIFSupportsReading(format))
		{
			std::cout << "Detected image format cannot be read!" << std::endl;
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
	}

	// If we're here we have a known image format, so load the image into a bitmap
	FIBITMAP* bitmap = FreeImage_Load(format, filename);

	// How many bits-per-pixel is the source image?
	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	// Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
	// but only if the image is not already 32 bits (i.e. 8 bits per channel).
	// Note: ConvertTo32Bits returns a CLONE of the image data - so if we
	// allocate this back to itself without using our bitmap32 intermediate
	// we will LEAK the original bitmap data, and valgrind will show things like this:
	//
	// LEAK SUMMARY:
	//  definitely lost: 24 bytes in 2 blocks
	//  indirectly lost: 1,024,874 bytes in 14 blocks    <--- Ouch.
	//
	// Using our intermediate and cleaning up the initial bitmap data we get:
	//
	// LEAK SUMMARY:
	//  definitely lost: 16 bytes in 1 blocks
	//  indirectly lost: 176 bytes in 4 blocks
	//
	// All above leaks (192 bytes) are caused by XGetDefault (in /usr/lib/libX11.so.6.3.0) - we have no control over this.
	//
	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32)
	{
		std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << std::endl;
		bitmap32 = bitmap;
	}
	else
	{
		std::cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << std::endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	// Some basic image info - strip it out if you don't care
	int imageWidth = FreeImage_GetWidth(bitmap32);
	int imageHeight = FreeImage_GetHeight(bitmap32);

	width = (unsigned int)imageWidth;
	height = (unsigned int)imageHeight;
	std::cout << "Image: " << filenameString << " is size: " << imageWidth << "x" << imageHeight << "." << std::endl;

	// Get a pointer to the texture data as an array of unsigned bytes.
	// Note: At this point bitmap32 ALWAYS holds a 32-bit colour version of our image - so we get our data from that.
	// Also, we don't need to delete or delete[] this textureData because it's not on the heap (so attempting to do
	// so will cause a crash) - just let it go out of scope and the memory will be returned to the stack.
	GLubyte* textureData = FreeImage_GetBits(bitmap32);

	return textureData;

	/*
	// Generate a texture ID and bind to it
	GLuint tempTextureID;
	glGenTextures(1, &tempTextureID);
	glBindTexture(GL_TEXTURE_2D, tempTextureID);



	// Construct the texture.
	// Note: The 'Data format' is the format of the image data as provided by the image library. FreeImage decodes images into
	// BGR/BGRA format, but we want to work with it in the more common RGBA format, so we specify the 'Internal format' as such.
	glTexImage2D(GL_TEXTURE_2D,    // Type of texture
		0,                // Mipmap level (0 being the top level i.e. full size)
		GL_RGBA,          // Internal format
		imageWidth,       // Width of the texture
		imageHeight,      // Height of the texture,
		0,                // Border in pixels
		GL_BGRA,          // Data format
		GL_UNSIGNED_BYTE, // Type of texture data
		textureData);     // The image data to use for this texture

						  // Specify our minification and magnification filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);

	// If we're using MipMaps, then we'll generate them here.
	// Note: The glGenerateMipmap call requires OpenGL 3.0 as a minimum.
	if (minificationFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minificationFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minificationFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minificationFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// Check for OpenGL texture creation errors
	GLenum glError = glGetError();
	if (glError)
	{
		std::cout << "There was an error loading the texture: " << filenameString << std::endl;

		switch (glError)
		{
		case GL_INVALID_ENUM:
			std::cout << "Invalid enum." << std::endl;
			break;

		case GL_INVALID_VALUE:
			std::cout << "Invalid value." << std::endl;
			break;

		case GL_INVALID_OPERATION:
			std::cout << "Invalid operation." << std::endl;

		default:
			std::cout << "Unrecognised GLenum." << std::endl;
			break;
		}

		std::cout << "See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details." << std::endl;
	}

	// Unload the 32-bit colour bitmap
	FreeImage_Unload(bitmap32);

	// If we had to do a conversion to 32-bit colour, then unload the original
	// non-32-bit-colour version of the image data too. Otherwise, bitmap32 and
	// bitmap point at the same data, and that data's already been free'd, so
	// don't attempt to free it again! (or we'll crash).
	if (bitsPerPixel != 32)
	{
		FreeImage_Unload(bitmap);
	}

	// Finally, return the texture ID
	return tempTextureID;
	
	
	*/

}
