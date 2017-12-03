#include "ogle/mesh.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "ogle/debug.hpp"

namespace {
std::istream &operator>>(std::istream &in, glm::vec3 &v) {
  in >> v.x;
  in >> v.y;
  in >> v.z;

  return in;
}
std::istream &operator>>(std::istream &in, glm::vec2 &v) {
  in >> v.x;
  in >> v.y;

  return in;
}
}

namespace ogle {

Mesh::Mesh(std::vector<ogle::Vertex> vertices,
           std::vector<unsigned int> indices,
           std::vector<ogle::Texture> textures)
    : vertices(vertices), indices(indices), textures(std::move(textures)) {
  bind_gl();
}

Mesh::Mesh(const std::string &filename) {
  std::ifstream f;
  f.open(filename);
  std::string s;
  while (std::getline(f, s)) {
    std::stringstream ss(s);
    char line_type;
    ss >> line_type;
    switch (line_type) {
    case '#': // comments
      std::cout << s << std::endl;
      break;
    case 'v': { // vertex info
      glm::vec3 pos, normal;
      glm::vec2 txt;
      ss >> pos >> normal >> txt;
      vertices.push_back(ogle::Vertex{pos, normal, txt});
      break;
    }
    case 'i': {
      unsigned int i;
      ss >> i;
      indices.push_back(i);
      break;
    }
    case 't': {
      std::string name, type;
      ss >> name >> type;
      ogle::Texture texture{name};
      texture.set_type(type);
      textures.push_back(std::move(texture));
      break;
    }
    }
  }

  bind_gl();
}

Mesh::~Mesh() {
  if (VAO) {
    DEBUG("Cleaning up VAO " << VAO);
    glDeleteVertexArrays(1, &VAO);
  }
  if (VBO) {
    DEBUG("Cleaning up VBO " << VBO);
    glDeleteBuffers(1, &VBO);
  }
  if (EBO) {
    DEBUG("Cleaning up EBO " << EBO);
    glDeleteBuffers(1, &EBO);
  }
}

void Mesh::Draw(ogle::Shader &shader) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    std::string name = textures[i].get_type();
    unsigned int n = 0;
    if (name == "texture_diffuse")
      n = diffuseNr++;
    else if (name == "texture_specular")
      n = specularNr++;

    shader.set_float(
        ("material." + textures[i].get_type() + std::to_string(n)).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].get_id());
  }
  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

  // for move
  Mesh::Mesh() : VAO(0), VBO(0), EBO(0) {}
  Mesh::Mesh(Mesh&& other)
  {
    std::swap(vertices, other.vertices);
    std::swap(indices, other.indices);
    std::swap(textures, other.textures);

    std::swap(VAO, other.VAO);
    std::swap(VBO, other.VBO);
    std::swap(EBO, other.EBO);
  }

  Mesh& Mesh::operator=(Mesh&& other)
  {
    std::swap(vertices, other.vertices);
    std::swap(indices, other.indices);
    std::swap(textures, other.textures);

    std::swap(VAO, other.VAO);
    std::swap(VBO, other.VBO);
    std::swap(EBO, other.EBO);
    
    return *this;
  }
  
void Mesh::bind_gl() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // vert positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  // vert normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));
  // vert texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}
}
