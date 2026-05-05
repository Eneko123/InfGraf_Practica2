#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 0.0f, 1.0f);
	aInstensity = 1.0f;

}

Light::Light(glm::vec3 color, GLfloat ambientIntensity)
{
	this->color = color;
	this->aInstensity = ambientIntensity;
}

void Light::UseLight(GLfloat colorLocation, GLfloat aIntensityLocation)
{
	glUniform1f(aIntensityLocation, aInstensity);
	glUniform3f(colorLocation, color.x, color.y, color.z);
}

Light::~Light()
{
}
