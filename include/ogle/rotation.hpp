#ifndef OGLE_ROTATION_HPP
#define OGLE_ROTATION_HPP

#include "glm/glm.hpp"

#include "ogle/rotaxis.hpp"

namespace ogle {

struct Rotation {
	float Degrees;
	glm::vec3 Axis;

	Rotation();
	Rotation(float Degrees, glm::vec3 Axis);
	Rotation(float Degrees, RotAxis axis);

};

}

#endif // OGLE_ROTATION_HPP
