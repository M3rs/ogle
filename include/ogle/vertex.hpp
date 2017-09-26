#ifndef OGLE_VERTEX_HPP
#define OGLE_VERTEX_HPP

#include "glm/glm.hpp"

namespace ogle {

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

}

#endif // OGLE_VERTEX_HPP
