#ifndef OGLE_DRAWING_BASICMODEL_HPP
#define OGLE_DRAWING_BASICMODEL_HPP

#include "ogle/drawing/drawable.hpp"

#include "ogle/transform.hpp"

namespace ogle {

class Model;
class Shader;

namespace drawing {

class BasicModel : public Drawable {
public:
  BasicModel(Model &model, Shader &shader, Transform transform);

  virtual void Draw();

private:
  Model& model;
  Shader& shader;
  Transform transform;
};
}
}

#endif // OGLE_DRAWING_BASICMODEL_HPP
