#pragma once
#include "Light.h"
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 dir);
	void UseLight(UniformDirectionalLight uniformDirLight );
private:
	glm::vec3 direction;

};

