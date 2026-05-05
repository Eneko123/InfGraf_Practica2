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

    //Tenemos que pasar el codigo como array
    const GLchar* code[1];
    code[0] = shaderCode;

    //Tenemos que pasar el tamaino del codigo como array
    GLint codeLenght[1];
    codeLenght[0] = (GLint)strlen(shaderCode);


    glShaderSource(shaderId, 1, code, codeLenght);//Metemos el codigo fuente en el shader
    glCompileShader(shaderId); //compilamos el shader

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


    uniformModel = glGetUniformLocation(idShader, "model");
    uniformProjection = glGetUniformLocation(idShader, "projection");
    uniformView = glGetUniformLocation(idShader, "view");
    uniformAmbientColor = glGetUniformLocation(idShader, "color");
    uniformAmbientIntensity = glGetUniformLocation(idShader, "ambientInten");
    uniformLightDir = glGetUniformLocation(idShader, "lightDir");
    uniformDiffuseInten = glGetUniformLocation(idShader, "diffuseInten");
}
