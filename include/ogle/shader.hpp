#ifndef OGLE_SHADER_HPP
#define OGLE_SHADER_HPP

#include "glad/glad.h"
#include <string>

#include "glm/glm.hpp"

namespace ogle {

// fwd
class Light;

class Shader {
public:
  Shader(const std::string &vert, const std::string &frag);
  ~Shader();

  void use();

  void set_int(const std::string &name, int i);
  void set_float(const std::string &name, float f);
  void set_vec3(const std::string &name, const glm::vec3 &vec3);
  void set_mat4(const std::string &name, const glm::mat4 &mat4);

  void set_light(const ogle::Light& light);
  
  // tmp ?
  GLuint getID() const;

private:
  GLuint ID;
};
}

#endif // OGLE_SHADER_HPP
