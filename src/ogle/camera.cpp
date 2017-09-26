#include "ogle/camera.hpp"

#include <SDL2/SDL.h>
#include "glm/gtc/matrix_transform.hpp"

namespace ogle {

Camera::Camera(int width, int height) 
	: m_pos(glm::vec3(0.0f, 0.0f, 8.0f))
	, m_front(glm::vec3(0.0f, 0.0f, -1.0f))
	, m_up(glm::vec3(0.0f, 1.0f, 0.0f)) 
  , m_yaw(-90.0f)
	, m_pitch(0.0f)
	, m_width(width)
	, m_height(height)
{ }

void Camera::update(float dt)
{
	// mouse / front update
	// TODO: refactor to fix mouse look
	// BUG - when the movement is *super* fast, the "hidden cursor" shows up big.
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	float xoff = mx - (m_width / 2);
	float yoff = (m_height / 2) - my;

	float sensativity = 0.07; // 0.05
	xoff *= sensativity;
	yoff *= sensativity;

	m_yaw += xoff;
	m_pitch += yoff;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	// keyboard update
	float speed = 3.00f; // px/s old: 2.00
	float dist = speed * dt;

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_W]) 
		m_pos += dist * m_front;
	if (keystate[SDL_SCANCODE_S])
		m_pos -= dist * m_front;
	if (keystate[SDL_SCANCODE_A])
		m_pos -= glm::normalize(glm::cross(m_front, m_up)) * dist;
	if (keystate[SDL_SCANCODE_D])
		m_pos += glm::normalize(glm::cross(m_front, m_up)) * dist;
	
	m_pos.y = 1.0f;
}

glm::mat4 Camera::get_view() const
{
	return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

glm::vec3 Camera::get_pos() const
{
	return m_pos;
}

glm::vec3 Camera::get_dir() const
{
	return m_front;
}

}
