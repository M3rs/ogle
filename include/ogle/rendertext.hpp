#ifndef OGLE_RENDERTEXT_HPP
#define OGLE_RENDERTEXT_HPP

#include <string>
#include "ogle/shader.hpp"
#include "ogle/font.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace ogle {
  
class RenderText {
public:
  RenderText(const std::string &text, ogle::Font& font, glm::mat4 projection);
  ~RenderText();

  void set_text(const std::string& text);
  void set_position(GLfloat x, GLfloat y);
  void set_color(const glm::vec3& color);
  void set_scale(GLfloat scale);

  void Draw(ogle::Shader &shader);

private:
  std::string text;
  ogle::Font& font;
  glm::vec3 color;
  GLuint VAO;
  GLuint VBO;

  GLfloat X;
  GLfloat Y;

  glm::mat4 projection;
  GLfloat scale;
};

}

#endif // OGLE_RENDERTEXT_HPP
