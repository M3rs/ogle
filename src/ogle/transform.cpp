#include "ogle/transform.hpp"

namespace ogle {

Transform::Transform()
  : Translation(glm::vec3(0, 0, 0)), Rotate(Rotation{}), Scale(glm::vec3(1,1,1)),
      Model(glm::mat4()) {
  calc_model();
}
  
Transform::Transform(glm::vec3 trans)
  : Translation(trans), Rotate(Rotation{}), Scale(glm::vec3(1, 1, 1)),
      Model(glm::mat4()) {
  calc_model();
}

Transform::Transform(glm::vec3 trans, float scale)
    : Translation(trans), Rotate(Rotation{}),
      Scale(glm::vec3(scale, scale, scale)), Model(glm::mat4()) {
  calc_model();
}

Transform::Transform(glm::vec3 trans, glm::vec3 scale)
    : Translation(trans), Rotate(Rotation{}), Scale(scale), Model(glm::mat4()) {
  calc_model();
}

Transform::Transform(glm::vec3 trans, Rotation rot, glm::vec3 scale)
    : Translation(trans), Rotate(rot), Scale(scale), Model(glm::mat4()) {
  calc_model();
}

void Transform::calc_model() {
  Model = glm::translate(Model, Translation);
  if (Rotate.Degrees != 0.0f)
    Model = glm::rotate(Model, glm::radians(Rotate.Degrees), Rotate.Axis);
  Model = glm::scale(Model, Scale);
}
}
