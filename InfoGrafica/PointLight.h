#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 pos, float constant, float linear, float exponential);
	void UseLight(UniformPointLight uniformPointLight);

private:
	glm::vec3 pos;
	float constantValue, linearValue, exponentialValue;
};

