#include "Camera.h"

Camera::Camera()
{
	isOrbital = false;
}

Camera::Camera(glm::vec3 startPos, float startYaw, float startPitch, float startMoveSpeed, float startTurnSpeed, glm::vec3 startWorldUp)
{
	pos = startPos;
	worldUp = startWorldUp;
	yaw = startYaw;
	pitch = startPitch;
	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	isOrbital = false;
	recalculate();
}

// Nuevo constructor para modo orbital
Camera::Camera(glm::vec3 target, float distance, float startYaw, float startPitch, float orbitSpeed, float zoomSpeed)
{
	this->target = target;
	this->distance = distance;
	this->yaw = startYaw;
	this->pitch = startPitch;
	this->orbitSpeed = orbitSpeed;
	this->zoomSpeed = zoomSpeed;
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->isOrbital = true;
	recalculateOrbital();
}

void Camera::update(InputManager& input, float deltaTime)
{
	if (isOrbital)
	{
		updateOrbital(input, deltaTime);
		return;
	}

	// Codigo original de camara libre
	if (input.isKeyPressed(GLFW_KEY_W)) pos += front * moveSpeed * deltaTime;
	if (input.isKeyPressed(GLFW_KEY_A)) pos -= right * moveSpeed * deltaTime;
	if (input.isKeyPressed(GLFW_KEY_S)) pos -= front * moveSpeed * deltaTime;
	if (input.isKeyPressed(GLFW_KEY_D)) pos += right * moveSpeed * deltaTime;
	if (input.isKeyPressed(GLFW_KEY_Q)) pos += up * moveSpeed * deltaTime;
	if (input.isKeyPressed(GLFW_KEY_E)) pos -= up * moveSpeed * deltaTime;

	yaw += input.getMouseDeltaX() * turnSpeed * deltaTime;
	pitch -= input.getMouseDeltaY() * turnSpeed * deltaTime;

	if (pitch > 89.9f) pitch = 89.9f;
	else if (pitch < -89.9f) pitch = -89.9f;
	recalculate();
}

void Camera::updateOrbital(InputManager& input, float deltaTime)
{
	// A/D para rotar horizontalmente (yaw)
	if (input.isKeyPressed(GLFW_KEY_A)) yaw -= orbitSpeed * deltaTime;
	if (input.isKeyPressed(GLFW_KEY_D)) yaw += orbitSpeed * deltaTime;

	// W/S para cambiar la distancia (zoom in/out)
	if (input.isKeyPressed(GLFW_KEY_W)) distance -= zoomSpeed * deltaTime;
	if (input.isKeyPressed(GLFW_KEY_S)) distance += zoomSpeed * deltaTime;

	// Limitar el zoom
	if (distance < 1.0f) distance = 1.0f;
	if (distance > 50.0f) distance = 50.0f;

	// Limitar el pitch
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	recalculateOrbital();
}

glm::mat4 Camera::getViewMatrix()
{
	if (isOrbital)
	{
		return glm::lookAt(pos, target, worldUp);
	}
	return glm::lookAt(pos, pos + front, up);
}

void Camera::recalculate()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::recalculateOrbital()
{
	// Calcular la posicion de la camara en coordenadas esfericas
	// x = distance * cos(pitch) * cos(yaw)
	// y = distance * sin(pitch)
	// z = distance * cos(pitch) * sin(yaw)

	float radiansYaw = glm::radians(yaw);
	float radiansPitch = glm::radians(pitch);

	pos.x = target.x + distance * cos(radiansPitch) * sin(radiansYaw);
	pos.y = target.y + distance * sin(radiansPitch);
	pos.z = target.z + distance * cos(radiansPitch) * cos(radiansYaw);

	// Calcular vectores de la cámara
	front = glm::normalize(target - pos);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
