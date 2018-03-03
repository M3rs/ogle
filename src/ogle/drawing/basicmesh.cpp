#include "ogle/drawing/basicmesh.hpp"
#include "ogle/mesh.hpp"
#include "ogle/shader.hpp"

namespace ogle {
namespace drawing {

BasicMesh::BasicMesh(Mesh &mesh, Shader &shader, Transform transform)
    : mesh(mesh), shader(shader), transform(transform) {}

void BasicMesh::Draw()
{
  shader.set_mat4("model", transform.Model);
  mesh.Draw(shader);
}
  
}
}
