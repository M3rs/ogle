#include "ogle/drawing/basicmodel.hpp"
#include "ogle/model.hpp"
#include "ogle/shader.hpp"

namespace ogle {
namespace drawing {

BasicModel::BasicModel(Model &model, Shader &shader, Transform transform)
  : model(model), shader(shader), transform(transform) {}

void BasicModel::Draw()
{
  shader.set_mat4("model", transform.Model);
  model.Draw(shader);
}
  
}
}
