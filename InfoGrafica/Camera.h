#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "InputManager.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPos, float startYaw, float startPitch, float startMoveSpeed, float startTurnSpeed, glm::vec3 startWorldUp);

	// Constructor para camara orbital
	Camera(glm::vec3 target, float distance, float startYaw, float startPitch, float orbitSpeed, float zoomSpeed);

	void update(InputManager& input, float deltaTime);
	void updateOrbital(InputManager& input, float deltaTime); // Nueva funcion para modo orbital

	glm::mat4 getViewMatrix();
	const glm::vec3& getCameraPos() { return pos; }

	// Setters para modo orbital
	void setOrbitalMode(bool orbital) { isOrbital = orbital; }
	void setTarget(glm::vec3 newTarget) { target = newTarget; }

private:
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw, pitch;
	float moveSpeed, turnSpeed;

	// Atributos para modo orbital
	bool isOrbital;
	glm::vec3 target;
	float distance;
	float orbitSpeed;
	float zoomSpeed;

	void recalculate();
	void recalculateOrbital(); // Nueva funcion para calcular posicion orbital
};
