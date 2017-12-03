#include "ogle/light.hpp"

namespace ogle {

Light::Light(const sol::table &t) {
  constant = t.get_or("constant", 0.0f);
  linear = t.get_or("linear", 0.0f);
  quadratic = t.get_or("quadratic", 0.0f);

  position = t.get<glm::vec3>("position");
  direction = t.get<glm::vec3>("direction");

  cutOff = t.get_or("cutOff", 0.0f);
  outerCutOff = t.get_or("outerCutOff", 0.0f);

  ambient = t.get<glm::vec3>("ambient");
  diffuse = t.get<glm::vec3>("diffuse");
  specular = t.get<glm::vec3>("specular");
}
  
}
