// InfoGrafica.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
#define _USE_MATH_DEFINES


#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "InputManager.h"
#include "Camera.h"
#include "Light.h"

const GLint WIDTH = 800, HEIGHT = 600;
const float TORADIANS = M_PI / 180.0f;

float posX = 0, speed = 2, posMax = 0.8;

float currentRot = 0, rotSpeed = 20, maxRot = 300;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;


InputManager input;
Camera mainCamera;
Light luzDireccional;
//Vertex shader
static const char* vShader = "Shaders/shader.vert";
//Fragment shader
static const char* fShader = "Shaders/shader.frag";


void CreateTriangle() {
    GLfloat vertices[] = {
//       x        y    z    nx     ny     nz
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };
    GLuint indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 2 ,1
    };
    Mesh* newMesh = new Mesh();
    newMesh->RecalculateNormals(vertices, indices, 24, 12, 6, 3);
    newMesh->CreateMesh(vertices, indices, 24, 12);
    meshList.push_back(newMesh);
}


void CreateShader() {
    Shader* shader = new Shader();
    shader->CreateShadersFromFile(vShader, fShader);
    shaderList.push_back(shader);
}

int main()
{
    Window mainWindow = Window(WIDTH, HEIGHT);
    mainWindow.Initialize();
    input = InputManager();
    input.Initialise(mainWindow.getWindow());
    mainCamera = Camera(
        glm::vec3(0, 0, 5.0f),
        -90.0f, 0.0f,
        5.0f,
        1.0f,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    luzDireccional = Light(glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);


    CreateShader();
    CreateTriangle();

    glm::mat4 projeccion = glm::perspective(glm::radians(45.0f), mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);


    float lastTime = (float)glfwGetTime();
    float deltaTime;


    while (!mainWindow.getWindowShouldClose()) {

        glfwPollEvents();

        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        mainCamera.update(input, deltaTime);


        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->useShader();


        luzDireccional.UseLight(shaderList[0]->GetIdAmbientColor(), shaderList[0]->GetIdAmbientIntensity());

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0, 0.0f, 0.0f));
        glUniformMatrix4fv(shaderList[0]->GetIdModel(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shaderList[0]->GetIdProjection(), 1, GL_FALSE, glm::value_ptr(projeccion));
        glUniformMatrix4fv(shaderList[0]->GetIdView(), 1, GL_FALSE, glm::value_ptr(mainCamera.getViewMatrix()));

        meshList[0]->RenderMesh();
        glUseProgram(0);

        mainWindow.swapBuffers();
    }

}