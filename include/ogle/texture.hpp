#ifndef OGLE_TEXTURE_HPP
#define OGLE_TEXTURE_HPP

#include <string>
#include "glad/glad.h"

namespace ogle {

class Texture {
public:

	Texture(const std::string &filename);
	~Texture();

	/// This does not support multiple textures yet!
	void bind();

	GLuint get_id();

	void set_type(const std::string &type);
	std::string get_type();

private:

	GLuint ID;
	std::string Type;

};

}

#endif // OGLE_TEXTURE_HPP
