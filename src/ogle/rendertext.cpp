#include "ogle/rendertext.hpp"

#include <iostream>

namespace ogle {

RenderText::RenderText(const std::string &text, ogle::Font &font,
                       glm::mat4 projection)
    : text(text), font(font), color(glm::vec3(0.0, 0.0, 0.0)),
      projection(projection), X(0.0), Y(0.0), scale(1.0) {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

RenderText::~RenderText() {
  if (VAO) {
    glDeleteVertexArrays(1, &VAO);
  }
  if (VBO) {
    glDeleteBuffers(1, &VBO);
  }
}

void RenderText::set_text(const std::string &text) { this->text = text; }

void RenderText::set_position(GLfloat x, GLfloat y) {
  X = x;
  Y = y;
}

void RenderText::set_color(const glm::vec3 &color) { this->color = color; }

void RenderText::set_scale(GLfloat scale) { this->scale = scale; }

void RenderText::Draw(ogle::Shader &shader) {
  GLfloat x = X;
  GLfloat y = Y;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader.use();
  shader.set_mat4("projection", projection);
  shader.set_vec3("textColor", color);
  // glActiveTexture(GL_TEXTURE0);
  // NB: Need to clean up VAO
  // Can I just re-use this for all text? (probably??)
  glBindVertexArray(VAO);

  auto &Characters = font.get_characters();

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    // ogle::Character &ch = Characters[*c];
    const ogle::Character &ch = Characters.at(*c);

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;

    // update for each char
    GLfloat vertices[6][4] = {
        {xpos, ypos + h, 0.0, 0.0}, {xpos, ypos, 0.0, 1.0},
        {xpos + w, ypos, 1.0, 1.0}, {xpos, ypos + h, 0.0, 0.0},
        {xpos + w, ypos, 1.0, 1.0}, {xpos + w, ypos + h, 1.0, 0.0}};

    // glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    ch.texture.bind();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // now advance cursor
    x += (ch.Advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_BLEND);
}
}
