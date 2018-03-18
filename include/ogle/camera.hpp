#ifndef OGLE_CAMERA_HPP
#define OGLE_CAMERA_HPP

#include "glm/glm.hpp"

namespace ogle {

// TODO: make Camera "interface", and player impl
class Camera {
public:
  Camera(int width, int height);

  void update(float dt);

  glm::mat4 get_view() const;
  glm::vec3 get_pos() const;
  glm::vec3 get_dir() const;

  glm::vec3 get_front() const;
  glm::vec3 get_up() const;
    
  void set_debug(bool debug);

private:
  glm::vec3 m_pos;
  glm::vec3 m_front;
  glm::vec3 m_up;

  float m_yaw;
  float m_pitch;

  int m_width;
  int m_height;

  bool debug;
  void get_offsets(float* xoff, float* yoff);
};
}

#endif // OGLE_CAMERA_HPP
