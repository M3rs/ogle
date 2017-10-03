#include "ogle/skybox.hpp"

#include "glad/glad.h"
#include "sol/helpers.hpp"
#include <string>

namespace ogle {

/// Assumes table has faces / vertices tables
Skybox::Skybox(sol::table skybox)
  : m_Texture(ogle::get_vector<std::string>(skybox["faces"]))
  , m_Shader(skybox["shader"]["vert"], skybox["shader"]["frag"])
{
  //m_Texture = Texture(ogle::get_vector<std::string>(skybox["faces"])));
  auto cverts = ogle::get_vector<float>(skybox["vertices"]);
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, cverts.size() * sizeof(float), &cverts[0],
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  //m_Shader = Shader{skybox["shader"]["vert"], skybox["shader"]["frag"]};
}

Skybox::~Skybox() {
  // TODO: clean up VAO, VBO, etc
}

void Skybox::Draw(glm::mat4 &view, glm::mat4 &projection) {
  glDepthFunc(GL_LEQUAL);
  m_Shader.use();
  m_Shader.set_float("skybox", 0);
  glm::mat4 cview = glm::mat4(glm::mat3(view));
  m_Shader.set_mat4("view", cview);
  m_Shader.set_mat4("projection", projection);

  glBindVertexArray(m_VAO);
  auto cubet = m_Texture.get_id();
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubet);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_LESS);
}

}
