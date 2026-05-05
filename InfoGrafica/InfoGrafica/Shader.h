#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>


class Shader
{
public:
	Shader();
	void CreateShadersFromString(const char* vertexCode, const char* fragmentCode);
	void CreateShadersFromFile(const char* vertexPath, const char* fragmentPath);

	GLuint GetIdModel() { return uniformModel; }
	GLuint GetIdProjection() { return uniformProjection; }
	GLuint GetIdView() { return uniformView; }
	GLuint GetIdAmbientColor() { return uniformAmbientColor; }
	GLuint GetIdAmbientIntensity() { return uniformAmbientIntensity; }
	GLuint GetIdLightDir() { return uniformLightDir; }
	GLuint GetIdDiffuseInten() { return uniformDiffuseInten; }



	void useShader();
	void deleteShader();
	~Shader();


private:
	GLuint idShader = 0, uniformModel = 0, uniformProjection = 0, uniformView = 0;
	GLuint uniformAmbientColor = 0, uniformAmbientIntensity = 0, uniformLightDir = 0, uniformDiffuseInten;
	std::string ReadFile(const char* path);
	void AddShader(const char* shaderCode, GLenum shaderType);
	void CompileShader(const char* vertexCode, const char* fragmentCode);


};

