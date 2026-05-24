#include "Shader.h"

Shader::Shader()
{
}

void Shader::CreateShadersFromString(const char* vertexCode, const char* fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateShadersFromFile(const char* vertexPath, const char* fragmentPath)
{
    std::string vS = ReadFile(vertexPath);
    std::string fS = ReadFile(fragmentPath);
    const char* vertexCode = vS.c_str();
    const char* fragmentCode = fS.c_str();

    CompileShader(vertexCode, fragmentCode);
}

void Shader::useShader()
{
    glUseProgram(idShader);
}

void Shader::deleteShader()
{
    if (idShader != 0) {
        glDeleteProgram(idShader);
        idShader = 0;
        uniformModel = 0;
        uniformProjection = 0;
    }

}

Shader::~Shader()
{
    deleteShader();
}



std::string Shader::ReadFile(const char* path)
{
    std::string content;
    std::fstream fileStream(path, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Error haciendo lectura de fichero en %s", path);
        return "";
    }
    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

void Shader::AddShader(const char* shaderCode, GLenum shaderType)
{
    GLuint shaderId = glCreateShader(shaderType); //Crea el shader y devuelve el id

    // Pasar el codigo como array
    const GLchar* code[1];
    code[0] = shaderCode;

    // Pasar el tamaino del codigo como array
    GLint codeLenght[1];
    codeLenght[0] = (GLint)strlen(shaderCode);


    glShaderSource(shaderId, 1, code, codeLenght);// Meter el codigo fuente en el shader
    glCompileShader(shaderId); // Compilar el shader

    GLint result = 0;
    GLchar eLog[1024] = {};
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result); //Miramos si ha funcionado la compilacion
    if (!result) {
        glGetShaderInfoLog(shaderId, sizeof(eLog), NULL, eLog); //sacamos el error
        printf("Error compilando el shader de %d: %s", shaderType, eLog);
        return;
    }
    glAttachShader(idShader, shaderId); //Enlaza el shader al program

}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    idShader = glCreateProgram();
    if (!idShader) {
        printf("Error al crear el shader program");
        return;
    }
    AddShader(vertexCode, GL_VERTEX_SHADER);
    AddShader(fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = {};
    glLinkProgram(idShader);
    glGetProgramiv(idShader, GL_LINK_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(idShader, sizeof(eLog), NULL, eLog); //sacamos el error
        printf("Error compilando el program: %s", eLog);
        return;
    }

    glValidateProgram(idShader);
    glGetProgramiv(idShader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(idShader, sizeof(eLog), NULL, eLog); //sacamos el error
        printf("Error validando el program: %s", eLog);
        return;
    }

    // Uniforms
    uniformModel = glGetUniformLocation(idShader, "model");
    uniformProjection = glGetUniformLocation(idShader, "projection");
    uniformView = glGetUniformLocation(idShader, "view");
    uniformCameraPos = glGetUniformLocation(idShader, "cameraPos");
    uniformTime = glGetUniformLocation(idShader, "time");

    uniformColorMap = glGetUniformLocation(idShader, "colorMap");
    uniformNormalMap = glGetUniformLocation(idShader, "normalMap");

    // Directional Light
    uniformDirLight.base.uniformColor = glGetUniformLocation(idShader, "directionalLight.base.color");
    uniformDirLight.base.uniformAmbientInten = glGetUniformLocation(idShader, "directionalLight.base.ambientInten");
    uniformDirLight.base.uniformDiffuseInten = glGetUniformLocation(idShader, "directionalLight.base.diffuseInten");
    uniformDirLight.uniformDir = glGetUniformLocation(idShader, "directionalLight.lightDir");

    // Point Light
    uniformPointLight.base.uniformColor = glGetUniformLocation(idShader, "pointLight.base.color");
    uniformPointLight.base.uniformAmbientInten = glGetUniformLocation(idShader, "pointLight.base.ambientInten");
    uniformPointLight.base.uniformDiffuseInten = glGetUniformLocation(idShader, "pointLight.base.diffuseInten");
    uniformPointLight.uniformPos = glGetUniformLocation(idShader, "pointLight.pos");
    uniformPointLight.uniformConstant = glGetUniformLocation(idShader, "pointLight.constV");
    uniformPointLight.uniformLinear = glGetUniformLocation(idShader, "pointLight.linearV");
    uniformPointLight.uniformExponential = glGetUniformLocation(idShader, "pointLight.exponentialV");

    // Material
    uniformShininess = glGetUniformLocation(idShader, "shininess");
    uniformSpecularInten = glGetUniformLocation(idShader, "specularInten");

    // Shadows
    uniformDirectionalLightTransform = glGetUniformLocation(idShader, "directionalLightTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(idShader, "directionalShadowMap");

    // Obtener localizaciones de SpotLights

    // Numero de spotlights activas
    uniformNumSpotLights = glGetUniformLocation(idShader, "numSpotLights");

    // Array de spotlights
    char uniformName[100];
    for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        // Base light (color, ambient, diffuse)
        sprintf_s(uniformName, "spotLights[%d].base.color", i);
        uniformSpotLights[i].base.uniformColor = glGetUniformLocation(idShader, uniformName);

        sprintf_s(uniformName, "spotLights[%d].base.ambientInten", i);
        uniformSpotLights[i].base.uniformAmbientInten = glGetUniformLocation(idShader, uniformName);

        sprintf_s(uniformName, "spotLights[%d].base.diffuseInten", i);
        uniformSpotLights[i].base.uniformDiffuseInten = glGetUniformLocation(idShader, uniformName);

        // Posición
        sprintf_s(uniformName, "spotLights[%d].pos", i);
        uniformSpotLights[i].uniformPos = glGetUniformLocation(idShader, uniformName);

        // Dirección
        sprintf_s(uniformName, "spotLights[%d].direction", i);
        uniformSpotLights[i].uniformDir = glGetUniformLocation(idShader, uniformName);

        // Atenuación
        sprintf_s(uniformName, "spotLights[%d].constV", i);
        uniformSpotLights[i].uniformConstant = glGetUniformLocation(idShader, uniformName);

        sprintf_s(uniformName, "spotLights[%d].linearV", i);
        uniformSpotLights[i].uniformLinear = glGetUniformLocation(idShader, uniformName);

        sprintf_s(uniformName, "spotLights[%d].exponentialV", i);
        uniformSpotLights[i].uniformExponential = glGetUniformLocation(idShader, uniformName);

        // Edge (coseno del angulo de corte)
        sprintf_s(uniformName, "spotLights[%d].edge", i);
        uniformSpotLights[i].uniformEdge = glGetUniformLocation(idShader, uniformName);
    }
}
