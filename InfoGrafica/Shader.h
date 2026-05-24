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

// Estructura para SpotLight
struct UniformSpotLight {
    UniformBaseLight base;
    GLuint uniformPos;
    GLuint uniformDir;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponential;
    GLuint uniformEdge;
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

    GLuint GetIdDirectionalLightTransform() { return uniformDirectionalLightTransform; }
    GLuint GetIdDirectionalShadowMap() { return uniformDirectionalShadowMap; }

    const UniformDirectionalLight GetIdDirectionalLight() { return uniformDirLight; }
    const UniformPointLight GetIdPointLight() { return uniformPointLight; }

    // Getters para SpotLights
    const UniformSpotLight* GetIdSpotLights() { return uniformSpotLights; }
    GLuint GetIdNumSpotLights() { return uniformNumSpotLights; }

    GLuint GetIdShininess() { return uniformShininess; }
    GLuint GetIdSpecularInten() { return uniformSpecularInten; }
    GLuint GetIdTime() { return uniformTime; }
    GLuint GetIdColorMap() { return uniformColorMap; }
    GLuint GetIdNormalMap() { return uniformNormalMap; }

    void useShader();
    void deleteShader();
    ~Shader();

private:
    GLuint idShader = 0;
    GLuint uniformModel = 0;
    GLuint uniformProjection = 0;
    GLuint uniformView = 0;
    GLuint uniformCameraPos = 0;

    GLuint uniformDirectionalLightTransform = 0;
    GLuint uniformDirectionalShadowMap = 0;

    GLuint uniformShininess = 0;
    GLuint uniformSpecularInten = 0;
    GLuint uniformTime = 0;
    GLuint uniformColorMap = 0;
    GLuint uniformNormalMap = 0;

    UniformDirectionalLight uniformDirLight;
    UniformPointLight uniformPointLight;

    // Array de SpotLights
    static const int MAX_SPOT_LIGHTS = 4;
    UniformSpotLight uniformSpotLights[MAX_SPOT_LIGHTS];
    GLuint uniformNumSpotLights;

    std::string ReadFile(const char* path);
    void AddShader(const char* shaderCode, GLenum shaderType);
    void CompileShader(const char* vertexCode, const char* fragmentCode);
};