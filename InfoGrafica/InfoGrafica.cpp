#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "InputManager.h"
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "OBJLoader.h"
#include "Texture.h"
#include "ShadowMap.h"

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

const float TORADIANS = M_PI / 180.0f;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

InputManager input;
Camera mainCamera;

DirectionalLight luzDireccional;
PointLight pointLight;

PointLight pointLightArray[TAM_POINT_LIGHTS];
unsigned int numPointLight = 0;

const int MAX_SPOT_LIGHTS = 4;
SpotLight spotLights[MAX_SPOT_LIGHTS];
int numSpotLights = 0;

Material mat;

Texture texturaEjemplo;
Texture texturaEjemploNormales;
Texture particleTexture;

ShadowMap directionalShadowMap;

static const char* vShader = "Shaders/shader.vs";
static const char* fShader = "Shaders/shader.fs";
static const char* directionalShadowMapVShader = "Shaders/directional_shadow_map.vert";
static const char* directionalShadowMapFShader = "Shaders/directional_shadow_map.frag";


void LoadModelFromOBJ(const char* path)
{
    std::vector<vertex_t> vertices;
    std::vector<GLuint> indices;

    if (!LoadOBJ(path, vertices, indices))
    {
        std::cout << "Error cargando OBJ: " << path << std::endl;
        return;
    }

    Mesh* newMesh = new Mesh();

    newMesh->CreateMeshFromVertices(
        vertices.data(),
        indices.data(),
        vertices.size(),
        indices.size()
    );

    meshList.push_back(newMesh);
}


void CreateModelsFromOBJ()
{
    LoadModelFromOBJ("Assets/Mierdon.obj");
    LoadModelFromOBJ("Assets/cubo.obj");
}


void CreateShader()
{
    Shader* shader = new Shader();
    shader->CreateShadersFromFile(vShader, fShader);
    shaderList.push_back(shader);
    Shader* particleShader = new Shader();
    shaderList.push_back(particleShader);

    Shader* shadowShader = new Shader();
    shadowShader->CreateShadersFromFile(
        directionalShadowMapVShader,
        directionalShadowMapFShader
    );
    shaderList.push_back(shadowShader);
}


int main()
{
    Window mainWindow = Window(WIDTH, HEIGHT);
    mainWindow.Initialize();

    input = InputManager();
    input.Initialise(mainWindow.getWindow());

    // Camara orbital
    mainCamera = Camera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        10.0f,
        -90.0f,
        20.0f,
        80.0f,
        10.0f
    );

    glm::vec3 dirLightDirection = glm::normalize(glm::vec3(-0.4f, -1.0f, 0.0f));

    // Luz direccional - Solo ambiente para ver el modelo
    luzDireccional = DirectionalLight(
        glm::vec3(1.0f, 1.0f, 1.0f),
        0.1f,
        0.0f,   // Sin diffuse direccional
        dirLightDirection
    );

    // Point light - desactivada
    pointLight = PointLight(
        glm::vec3(1.0f, 1.0f, 1.0f),
        0.0f,
        0.0f,
        glm::vec3(-2.0f, 2.0f, -3.0f),
        0.01f,
        1.0f,
        1.0f
    );

    // Spotligths
    glm::vec3 modelCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    // SpotLight 1 - Frontal
    spotLights[0] = SpotLight(
        glm::vec3(1.0f, 1.0f, 1.0f),       // Blanco
        0.2f,                               // Algo de ambient
        1.0f,                               // Diffuse
        glm::vec3(0.0f, 10.0f, 8.0f),      // Frontal alto
        glm::vec3(0.0f, -1.0f, 0.0f),
        1.0f,
        0.05f,                             // Linear 
        0.001f,                             // Exponential 
        50.0f                               // Ángulo amplio (50°)
    );
    spotLights[0].PointTo(modelCenter);

    // SpotLight 2 - Lateral izquierdo
    spotLights[1] = SpotLight(
        glm::vec3(0.3f, 0.5f, 1.0f),       // Azul 
        0.1f,
        1.0f,                               // Intensidad
        glm::vec3(-8.0f, 8.0f, 2.0f),      // Mas alejado
        glm::vec3(0.0f, -1.0f, 0.0f),
        1.0f,
        0.005f,
        0.001f,
        50.0f
    );
    spotLights[1].PointTo(modelCenter);

    // SpotLight 3 - Lateral derecho 
    spotLights[2] = SpotLight(
        glm::vec3(1.0f, 0.5f, 0.2f),       // Naranja 
        0.1f,
        1.0f,
        glm::vec3(8.0f, 8.0f, -2.0f),      // Mas alejado
        glm::vec3(0.0f, -1.0f, 0.0f),
        1.0f,
        0.005f,
        0.001f,
        50.0f
    );
    spotLights[2].PointTo(modelCenter);

    numSpotLights = 3;

    // Material
    mat = Material(8.0f, 256.0f);

    texturaEjemplo = Texture(
        "Assets/Textures/marble_cliff_02_1k/textures/marble_cliff_02_diff_1k.jpg"
    );
    texturaEjemplo.LoadTexture();

    texturaEjemploNormales = Texture(
        "Assets/Textures/marble_cliff_02_1k/textures/marble_cliff_02_nor_gl_1k.jpg"
    );
    texturaEjemploNormales.LoadTexture();

    particleTexture = Texture("Assets/Textures/particle.png");
    particleTexture.LoadTexture();

    CreateShader();

    if (!directionalShadowMap.Init(SHADOW_WIDTH, SHADOW_HEIGHT))
    {
        std::cout << "Error inicializando directionalShadowMap" << std::endl;
        return -1;
    }

    CreateModelsFromOBJ();

    glm::mat4 projeccion = glm::perspective(
        glm::radians(45.0f),
        mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f,
        100.0f
    );

    glEnable(GL_DEPTH_TEST);

    float lastTime = (float)glfwGetTime();
    float deltaTime = 0.0f;

    std::cout << "  PRACTICA 2 - camara orbital + spotlights" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "  A/D - Rotar horizontalmente" << std::endl;
    std::cout << "  W/S - Zoom in/out" << std::endl;
    std::cout << "\nSpotLights activos: " << numSpotLights << std::endl;

    while (!mainWindow.getWindowShouldClose())
    {
        glfwPollEvents();

        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        mainCamera.update(input, deltaTime);

        glm::mat4 modelPrincipal(1.0f);
        modelPrincipal = glm::translate(modelPrincipal, glm::vec3(0.0f, 0.0f, 0.0f));

        glm::mat4 modelSuelo(1.0f);
        modelSuelo = glm::translate(modelSuelo, glm::vec3(0.0f, -2.0f, 0.0f));
        modelSuelo = glm::scale(modelSuelo, glm::vec3(10.0f, 0.1f, 10.0f));

        glm::mat4 lightProjection = glm::ortho(
            -15.0f, 15.0f,
            -15.0f, 15.0f,
            0.1f, 40.0f
        );

        glm::mat4 lightView = glm::lookAt(
            -dirLightDirection * 15.0f,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        glm::mat4 directionalLightTransform = lightProjection * lightView;

        // Shadow map
        directionalShadowMap.Write();
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glClear(GL_DEPTH_BUFFER_BIT);

        shaderList[2]->useShader();
        glUniformMatrix4fv(
            shaderList[2]->GetIdDirectionalLightTransform(),
            1,
            GL_FALSE,
            glm::value_ptr(directionalLightTransform)
        );

        if (meshList.size() > 0)
        {
            glUniformMatrix4fv(
                shaderList[2]->GetIdModel(),
                1,
                GL_FALSE,
                glm::value_ptr(modelPrincipal)
            );
            meshList[0]->RenderMesh();
        }

        if (meshList.size() > 1)
        {
            glUniformMatrix4fv(
                shaderList[2]->GetIdModel(),
                1,
                GL_FALSE,
                glm::value_ptr(modelSuelo)
            );
            meshList[1]->RenderMesh();
        }

        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Render normal
        glViewport(
            0,
            0,
            mainWindow.getBufferWidth(),
            mainWindow.getBufferHeight()
        );

        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->useShader();

        // Enviar luces
        luzDireccional.UseLight(shaderList[0]->GetIdDirectionalLight());
        pointLight.UseLight(shaderList[0]->GetIdPointLight());

        // Spotlights
        glUniform1i(shaderList[0]->GetIdNumSpotLights(), numSpotLights);
        const UniformSpotLight* uniformSpots = shaderList[0]->GetIdSpotLights();
        for (int i = 0; i < numSpotLights; i++)
        {
            spotLights[i].UseLight(uniformSpots[i]);
        }

        // Texturas
        texturaEjemplo.UseTexture(0);
        glUniform1i(shaderList[0]->GetIdColorMap(), 0);

        texturaEjemploNormales.UseTexture(1);
        glUniform1i(shaderList[0]->GetIdNormalMap(), 1);

        directionalShadowMap.Read(GL_TEXTURE3);
        glUniform1i(shaderList[0]->GetIdDirectionalShadowMap(), 3);

        // Matrices
        glUniformMatrix4fv(
            shaderList[0]->GetIdDirectionalLightTransform(),
            1,
            GL_FALSE,
            glm::value_ptr(directionalLightTransform)
        );

        glUniformMatrix4fv(
            shaderList[0]->GetIdProjection(),
            1,
            GL_FALSE,
            glm::value_ptr(projeccion)
        );

        glUniformMatrix4fv(
            shaderList[0]->GetIdView(),
            1,
            GL_FALSE,
            glm::value_ptr(mainCamera.getViewMatrix())
        );

        glUniform3fv(
            shaderList[0]->GetIdCameraPos(),
            1,
            glm::value_ptr(mainCamera.getCameraPos())
        );

        glUniform1f(
            shaderList[0]->GetIdTime(),
            currentTime
        );

        mat.UseMaterial(
            shaderList[0]->GetIdSpecularInten(),
            shaderList[0]->GetIdShininess()
        );

        // Render modelo
        if (meshList.size() > 0)
        {
            glUniformMatrix4fv(
                shaderList[0]->GetIdModel(),
                1,
                GL_FALSE,
                glm::value_ptr(modelPrincipal)
            );
            meshList[0]->RenderMesh();
        }

        // Render suelo
        if (meshList.size() > 1)
        {
            glUniformMatrix4fv(
                shaderList[0]->GetIdModel(),
                1,
                GL_FALSE,
                glm::value_ptr(modelSuelo)
            );
            meshList[1]->RenderMesh();
        }

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}