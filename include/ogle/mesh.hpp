#ifndef OGLE_MESH_HPP
#define OGLE_MESH_HPP

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "ogle/vertex.hpp"
#include "ogle/texture.hpp"
#include "ogle/shader.hpp"

namespace ogle {

class Mesh {
public:
	/* mesh data */
	// TODO: move to private, add accessors?
	std::vector<ogle::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ogle::Texture> textures;

	Mesh(std::vector<ogle::Vertex> vertices, std::vector<unsigned int> indices, std::vector<ogle::Texture> textures);

	// for "omf" data files
	Mesh(const std::string &filename);

	void Draw(ogle::Shader &shader);

private:
	/* render data */
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void bind_gl();

};

}

#endif // OGLE_MESH_HPP
