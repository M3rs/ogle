#include "ogle/rotation.hpp"

namespace ogle {

Rotation::Rotation() : Degrees(0.0f) {}

Rotation::Rotation(float Degrees, glm::vec3 Axis)
    : Degrees(Degrees), Axis(Axis) {}

Rotation::Rotation(float Degrees, RotAxis axis) : Degrees(Degrees) {
  switch (axis) {
  case RotAxis::X:
    Axis = glm::vec3(1.0f, 0.0f, 0.0f);
    break;
  case RotAxis::Y:
    Axis = glm::vec3(0.0f, 1.0f, 0.0f);
    break;
  case RotAxis::Z:
    Axis = glm::vec3(0.0f, 0.0f, 1.0f);
    break;
  default:
    Axis = glm::vec3();
    break;
  }
}
}
