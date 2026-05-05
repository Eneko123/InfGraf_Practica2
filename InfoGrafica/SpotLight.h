#pragma once
#include "Light.h"
class SpotLight : public Light
{
public:
	SpotLight();
	SpotLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 dir, glm::vec3 angle);
	void UseLight(UniformSpotLight uniformSpotLight);
private:
	glm::vec3 direction;

};

