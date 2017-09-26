#ifndef OGLE_SHADER_HPP
#define OGLE_SHADER_HPP

#include <string>
#include "glad/glad.h"

#include "glm/glm.hpp"

namespace ogle {

class Shader {
public:

	Shader(const std::string &vert, const std::string &frag);
	~Shader();

	void use();

	void set_int(const std::string &name, int i);
	void set_float(const std::string &name, float f);
	void set_vec3(const std::string &name, const glm::vec3 &vec3);
	void set_mat4(const std::string &name, const glm::mat4 &mat4);

	// tmp ?
	GLuint getID() const;

private:

	GLuint ID;

};

}

#endif // OGLE_SHADER_HPP
