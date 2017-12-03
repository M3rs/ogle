#ifndef OGLE_LIGHT_HPP
#define OGLE_LIGHT_HPP

#include "glm/glm.hpp"
#include "sol/sol.hpp"

namespace ogle {
  
class Light {
public:

  glm::vec3 position;
  glm::vec3 direction;

  float cutOff;
  float outerCutOff;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;

  // ctors
  Light(const sol::table& t);
  
  // methods
  //void update(const glm::vec3& pos, const glm::vec3& dir);
  
};

}

#endif // OGLE_LIGHT_HPP
