#version 330 core
out vec4 FragColor;

// NB: sampler2D is an "opaque type", cannot be used except in uniformss
struct Material {
	//sampler2D diffuse;
	sampler2D texture_diffuse1;
	//sampler2D texture_diffuse2;
	//sampler2D specular;
	sampler2D texture_specular1;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// attenuation
	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// ambient
	//vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;;
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoords).rgb;;

	// specular 
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;


	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
