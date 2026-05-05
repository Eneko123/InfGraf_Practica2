#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 0.0f, 1.0f);
	aInstensity = 0.0f;
	dIntensity = 0.0f;

}

Light::Light(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity)
{
	this->color = color;
	this->aInstensity = ambientIntensity;
	dIntensity = diffuseIntensity;
}

Light::~Light()
{
}

void Light::UseLight(UniformBaseLight baseLight)
{
	glUniform1f(baseLight.uniformAmbientInten, aInstensity);
	glUniform1f(baseLight.uniformDiffuseInten, dIntensity);
	glUniform3f(baseLight.uniformColor, color.x, color.y, color.z);
}
