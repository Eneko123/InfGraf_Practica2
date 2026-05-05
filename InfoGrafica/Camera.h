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
	void update(InputManager& input, float deltaTime);
	glm::mat4 getViewMatrix();
	const glm::vec3& getCameraPos() { return pos; }

private:
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw, pitch;
	float moveSpeed, turnSpeed;
	void recalculate();
};

