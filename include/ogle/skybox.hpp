#ifndef OGLE_SKYBOX_HPP
#define OGLE_SKYBOX_HPP

#include <vector>

#include "glm/glm.hpp"
#include "sol/sol.hpp"

#include "ogle/texture.hpp"
#include "ogle/shader.hpp"

namespace ogle {

class Skybox {
public:

  // TODO: other non-lua based ctor
  
  Skybox(sol::table skybox);
  ~Skybox();
  
  void Draw(glm::mat4 &view, glm::mat4 &projection);

  // TODO: move ctor / assignment ??

private:
  GLuint m_VAO;
  GLuint m_VBO;

  ogle::Texture m_Texture;
  ogle::Shader m_Shader;
};
}

#endif // OGLE_SKYBOX_HPP
