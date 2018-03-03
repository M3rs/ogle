#ifndef OGLE_DRAWING_BASICMESH_HPP
#define OGLE_DRAWING_BASICMESH_HPP

#include "ogle/drawing/drawable.hpp"
#include "ogle/transform.hpp"

namespace ogle {
  class Mesh;
  class Shader;

  namespace drawing {
    
class BasicMesh : public Drawable {
public:
  BasicMesh(Mesh &mesh, Shader& shader, Transform transform);

  virtual void Draw();
private:
  Mesh& mesh;
  Shader& shader;
  Transform transform;
};

  }
}

#endif // OGLE_DRAWING_BASICMESH_HPP
