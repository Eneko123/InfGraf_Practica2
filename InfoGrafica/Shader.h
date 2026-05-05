#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct UniformBaseLight {
	GLuint uniformColor;
	GLuint uniformAmbientInten;
	GLuint uniformDiffuseInten;
	
};
struct UniformDirectionalLight {
	
	UniformBaseLight base;
	GLuint uniformDir;
};

struct UniformPointLight {

	UniformBaseLight base;
	GLuint uniformPos;
	GLuint uniformConstant;
	GLuint uniformLinear;
	GLuint uniformExponential;
};
class Shader
{
public:
	Shader();
	void CreateShadersFromString(const char* vertexCode, const char* fragmentCode);
	void CreateShadersFromFile(const char* vertexPath, const char* fragmentPath);

	GLuint GetIdModel() { return uniformModel; }
	GLuint GetIdProjection() { return uniformProjection; }
	GLuint GetIdView() { return uniformView; }
	GLuint GetIdCameraPos() { return uniformCameraPos; }
	const UniformDirectionalLight GetIdDirectionalLight() { return uniformDirLight; }
	const UniformPointLight GetIdPointLight() { return uniformPointLight; }
	GLuint GetIdShininess() { return uniformShininess; }
	GLuint GetIdSpecularInten() { return uniformSpecularInten; }
	GLuint GetIdTime() { return uniformTime; }
	GLuint GetIdColorMap() { return uniformColorMap; }


	void useShader();
	void deleteShader();
	~Shader();


private:
	GLuint idShader = 0, uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformCameraPos = 0;
	GLuint uniformShininess, uniformSpecularInten;
	GLuint uniformTime = 0;
	GLuint uniformColorMap;
	UniformDirectionalLight uniformDirLight;
	UniformPointLight uniformPointLight;

	std::string ReadFile(const char* path);
	void AddShader(const char* shaderCode, GLenum shaderType);
	void CompileShader(const char* vertexCode, const char* fragmentCode);


};

