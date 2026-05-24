#include "SpotLight.h"

SpotLight::SpotLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	constantValue = 1.0f;
	linearValue = 0.0f;
	exponentialValue = 0.0f;
	edge = 0.0f;
}

SpotLight::SpotLight(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity,
	glm::vec3 position, glm::vec3 direction,
	GLfloat constant, GLfloat linear, GLfloat exponential,
	GLfloat edgeAngle)
	: Light(color, ambientIntensity, diffuseIntensity)
{
	this->position = position;
	this->direction = glm::normalize(direction);
	this->constantValue = constant;
	this->linearValue = linear;
	this->exponentialValue = exponential;

	// Convertir el angulo a coseno para el shader
	this->edge = cos(glm::radians(edgeAngle));
}

void SpotLight::UseLight(UniformSpotLight uniformSpotLight)
{
	// Enviar los datos base de la luz (color, intensidades)
	Light::UseLight(uniformSpotLight.base);

	// Enviar posicion
	glUniform3f(uniformSpotLight.uniformPos, position.x, position.y, position.z);

	// Enviar direccion
	glUniform3f(uniformSpotLight.uniformDir, direction.x, direction.y, direction.z);

	// Enviar valores de atenuacion
	glUniform1f(uniformSpotLight.uniformConstant, constantValue);
	glUniform1f(uniformSpotLight.uniformLinear, linearValue);
	glUniform1f(uniformSpotLight.uniformExponential, exponentialValue);

	// Enviar angulo de corte (coseno)
	glUniform1f(uniformSpotLight.uniformEdge, edge);
}

void SpotLight::PointTo(glm::vec3 target)
{
	// Calcular la direccion desde la posicion del foco hacia el target
	direction = glm::normalize(target - position);
}

SpotLight::~SpotLight()
{
}
