#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
	//FragColor = texture(ourTexture, TexCoord);

	// testing ambient lighting
	float amb = 0.8;
	
	vec4 result = texture(ourTexture, TexCoord) * amb;

	FragColor = result;

}
