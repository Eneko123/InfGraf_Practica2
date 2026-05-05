#include "PointLight.h"

PointLight::PointLight() : Light()
{
	pos = glm::vec3(0);
	constantValue = 0;
	linearValue = 0;
	exponentialValue = 0;
}

PointLight::PointLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 pos, float constant, float linear, float exponential)
	: Light(color, ambientIntensity, diffuseIntensity)
{
	this->pos = pos;
	constantValue = constant;
	linearValue = linear;
	exponentialValue = exponential;
}

void PointLight::UseLight(UniformPointLight uniformPointLight)
{
	Light::UseLight(uniformPointLight.base);
	glUniform3f(uniformPointLight.uniformPos, pos.x, pos.y, pos.z);
	glUniform1f(uniformPointLight.uniformExponential, exponentialValue);
	glUniform1f(uniformPointLight.uniformLinear, linearValue);
	glUniform1f(uniformPointLight.uniformConstant, constantValue);


}
