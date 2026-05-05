#include "Window.h"

Window::Window(GLint width, GLint height)
{
	this->width = width;
	this->height = height;
}

int Window::Initialize()
{
    if (!glfwInit()) {
        printf("Inicializacion de GLFW ha fallado");
        glfwTerminate();
        return 1;
    }
    //Connfiguracion de las ventanas de GLFW
    //Version de OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Utilizamos el core profile y no permitimos compatibilidad con cosas obsoletas
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    window = glfwCreateWindow(width, height, "Ventana prueba", NULL, NULL);

    if (!window) {
        printf("Error al crear la ventana");
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("GLEW ha fallado al inicializarse");
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);
    return 0;
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
