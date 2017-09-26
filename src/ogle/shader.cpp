#include "ogle/shader.hpp"

// more GLM
#include <glm/gtc/type_ptr.hpp>

// helper methods for making shaders
#include <iostream>
#include <iterator>
#include <fstream>
std::string read_file(const std::string &filename)
{
	std::ifstream f{filename};
	using Fbuf = std::istreambuf_iterator<char>;
	return std::string { Fbuf(f),Fbuf() };
}

void check_shader(unsigned int shader)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Error compiling shader:\n" << infoLog << std::endl;
	}
}

GLuint load_shader(const std::string &filename, GLenum type)
{
	GLuint shader = glCreateShader(type);

	std::string vsrc = read_file(filename);
	const char* vss = vsrc.c_str();
	glShaderSource(shader, 1, &vss, NULL);
	glCompileShader(shader);

	check_shader(shader);

	return shader;
}

namespace ogle {

Shader::Shader(const std::string &vert, const std::string &frag)
{
	GLuint vertShader = load_shader(vert, GL_VERTEX_SHADER);
	GLuint fragShader = load_shader(frag, GL_FRAGMENT_SHADER);
	ID = glCreateProgram();
	glAttachShader(ID, vertShader);
	glAttachShader(ID, fragShader);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Shader Program Error:\n" << infoLog << std::endl;
	}

	// after linked to shader, we don't need the actual shaders
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

Shader::~Shader()
{
	if (ID) {
		glDeleteShader(ID);
	}
}

void Shader::use()
{
	glUseProgram(ID);
}

// TODO: clean up all the getUniform calls
void Shader::set_int(const std::string &name, int i)
{
	int loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cout << "Could not find uniform (" << name << ") in shader program (" << ID << ")" << std::endl;
		return;
	}
	glUniform1i(loc, i);
}

void Shader::set_float(const std::string &name, float f)
{
	//std::cout << "DEBUG: setting " << name << " to " << f << std::endl;
	int loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cout << "Could not find uniform (" << name << ") in shader program (" << ID << ")" << std::endl;
		return;
	}

	glUniform1f(loc, f);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &vec3)
{
	int loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cout << "Could not find uniform (" << name << ") in shader program (" << ID << ")" << std::endl;
		return;
	}
	
	glUniform3fv(loc, 1, glm::value_ptr(vec3));
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &mat4)
{
	int loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cout << "Could not find uniform (" << name << ") in shader program (" << ID << ")" << std::endl;
		return;
	}
	
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
}

GLuint Shader::getID() const
{
	return ID;
}

}
