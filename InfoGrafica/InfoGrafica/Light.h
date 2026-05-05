#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
class Light
{
public:
	Light();
	Light(glm::vec3 color, GLfloat ambientIntensity);
	void UseLight(GLfloat colorLocation, GLfloat aIntensityLocation);
	~Light();
private:
	glm::vec3 color;
	GLfloat aInstensity;
};

