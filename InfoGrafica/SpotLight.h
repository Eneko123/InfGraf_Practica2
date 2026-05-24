#pragma once
#include "Light.h"

class SpotLight : public Light
{
public:
	SpotLight();
	SpotLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity,
		glm::vec3 position, glm::vec3 direction,
		GLfloat constant, GLfloat linear, GLfloat exponential,
		GLfloat edgeAngle);

	void UseLight(UniformSpotLight uniformSpotLight);
	void PointTo(glm::vec3 target);

	~SpotLight();

private:
	glm::vec3 position;
	glm::vec3 direction;

	// Atenuacion (igual que PointLight)
	GLfloat constantValue;
	GLfloat linearValue;
	GLfloat exponentialValue;

	// Angulo de corte del cono
	GLfloat edge;
};
