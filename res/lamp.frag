#version 330 core
out vec4 FragColor;

in vec3 lColor;

void main()
{
	FragColor = vec4(lColor, 1.0); // set to white
}
