#pragma once
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Window
{
public:
	Window(GLint width, GLint height);
	int Initialize();
	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }
	void swapBuffers() { glfwSwapBuffers(window); }
	bool getWindowShouldClose() { return glfwWindowShouldClose(window); }
	GLFWwindow* getWindow() { return window; }
	~Window();
private:
	GLFWwindow* window;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

};

