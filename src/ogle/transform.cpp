#include "ogle/transform.hpp"

namespace ogle {

Transform::Transform(glm::vec3 trans)
	: Translation(trans)
	, Rotate(Rotation{})
	, Scale(glm::vec3())
	, Model(glm::mat4())
{
	calc_model();
}

Transform::Transform(glm::vec3 trans, glm::vec3 scale)
	: Translation(trans)
	, Rotate(Rotation{})
	, Scale(scale)
	, Model(glm::mat4())
{
	calc_model();
}

Transform::Transform(glm::vec3 trans, Rotation rot, glm::vec3 scale)
	: Translation(trans)
	, Rotate(rot)
	, Scale(scale)
	, Model(glm::mat4())
{
	calc_model();
}

void Transform::calc_model()
{
	Model = glm::translate(Model, Translation);
	if (Rotate.Degrees != 0.0f)
		Model = glm::rotate(Model, glm::radians(Rotate.Degrees), Rotate.Axis);
	Model = glm::scale(Model, Scale);

}

}
