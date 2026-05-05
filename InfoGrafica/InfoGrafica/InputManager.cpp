#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
{
	window = nullptr;
	lastX = 0.0f;
	lastY = 0.0f;
	deltaX = 0.0f;
	deltaY = 0.0f;
}

void InputManager::Initialise(GLFWwindow* newwindow)
{
	window = newwindow;
	instance = this;
	glfwSetKeyCallback(window, handleKeys);
	glfwSetCursorPosCallback(window, handleMouse);


}

bool InputManager::isKeyPressed(int key)
{
	auto it = keyState.find(key);
	if (it != keyState.end()) {
		return it->second;
	}
	return false;
}

void InputManager::handleKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!instance) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (action == GLFW_PRESS) instance->keyState[key] = true;
	else if (action == GLFW_RELEASE) instance->keyState[key] = false;

}

void InputManager::handleMouse(GLFWwindow* window, double posX, double posY)
{
	if (!instance) return;
	if (instance->firstMove) {
		instance->lastX = posX;
		instance->lastY = posY;
		instance->deltaX = 0.0f;
		instance->deltaY = 0.0f;
		instance->firstMove = false;
		return;
	}
	instance->deltaX = posX - instance->lastX;
	instance->deltaY = posY - instance->lastY;

	instance->lastX = posX;
	instance->lastY = posY;
}


