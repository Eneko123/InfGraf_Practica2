#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
class Light
{
public:
	Light();
	Light(glm::vec3 color, GLfloat ambientIntensity, GLfloat diffuseIntensity);
	~Light();
protected:
	void UseLight(UniformBaseLight baseLight);
	glm::vec3 color;
	GLfloat aInstensity;
	GLfloat dIntensity;
};

