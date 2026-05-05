#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 dir)
	: Light(color, ambientIntensity, diffuseIntensity)
{
	direction = dir;
}

void DirectionalLight::UseLight(UniformDirectionalLight uniformDirLight)
{
	Light::UseLight(uniformDirLight.base);
	glUniform3f(uniformDirLight.uniformDir, direction.x, direction.y, direction.z);


}
