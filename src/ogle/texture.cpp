#include "ogle/texture.hpp"

#include <iostream>
#include "stb/stb_image.h"

namespace ogle {

Texture::Texture(const std::string &filename) 
{
	GLuint texture;
	glGenTextures(1, &texture);
	std::cout << "DEBUG: Loading texture '" << filename << "' as texture " << texture << std::endl;
	glBindTexture(GL_TEXTURE_2D, texture);
	// set texture wrap/filter options (on the current bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texure
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels; // what are nrChannels
	// nrChannels appears to be color type
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		if (nrChannels == 3)
			format = GL_RGB;
		if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cout << "Failed to load texture: " << filename << std::endl;
	}
	stbi_image_free(data);

	ID = texture;
}


Texture::Texture(const std::vector<std::string> faces)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			if (nrChannels == 3)
				format = GL_RGB;
			if (nrChannels == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			//glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load texture: " << faces[i] << std::endl;
			// should it die here?
		}
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	ID = texture;
}

Texture::~Texture()
{
	//std::cout << "destructor! ah ha" << std::endl;
	// TODO: add clean up, fix copying these
	//glDeleteTextures(1, &ID);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

GLuint Texture::get_id()
{
	return ID;
}

void Texture::set_type(const std::string &type)
{
	Type = type;
}

std::string Texture::get_type()
{
	return Type;
}


}
