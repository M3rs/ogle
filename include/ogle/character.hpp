#ifndef OGLE_CHARACTER_HPP
#define OGLE_CHARACTER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "ogle/texture.hpp"

namespace ogle {

struct Character {
  Texture texture;
  glm::vec2 Size;
  glm::vec2 Bearing;
  GLuint Advance;
};

}

#endif // OGLE_CHARACTER_HPP
