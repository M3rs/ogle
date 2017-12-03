#ifndef OGLE_TRANSFORM_HPP
#define OGLE_TRANSFORM_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ogle/rotation.hpp"

namespace ogle {

class Transform {
public:
  Transform(glm::vec3 trans);
  Transform(glm::vec3 trans, glm::vec3 scale);
  Transform(glm::vec3 trans, Rotation rot, glm::vec3 scale);

  glm::vec3 Translation;
  Rotation Rotate;
  glm::vec3 Scale;
  glm::mat4 Model; // todo - make a "get" method?

private:
  void calc_model();
};
}

#endif // OGLE_TRANSFORM_HPP
