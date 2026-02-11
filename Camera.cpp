#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

const float WINDOW_WIDTH = 800.0f;
const float WINDOW_HEIGHT = 800.0f;

Camera::Camera(const glm::vec3& pos, const glm::vec3& worldUp) :
	position(pos),
	yaw(-90.0f),
	pitch(0.0f),
	worldUp(worldUp),
	sensitivity(0.1f),
	speed(0.05f),
	fov(60.0f),
	aspect(WINDOW_WIDTH / WINDOW_HEIGHT),
	nearPlane(0.01f),
	farPlane(100.0f)
{
	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	float yawRad = glm::radians(yaw);
	float pitchRad = glm::radians(pitch);

	glm::vec3 newFront;
	newFront.x = std::cos(pitchRad) * std::cos(yawRad);
	newFront.y = std::sin(pitchRad);
	newFront.z = std::cos(pitchRad) * std::sin(yawRad);
	front = glm::normalize(newFront);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::moveForward(float distance)
{
	position += front * distance * speed;
}

void Camera::moveRight(float distance)
{
	position += right * distance * speed;
}

void Camera::moveUp(float distance)
{
	position += worldUp * distance * speed;
}

void Camera::rotate(float xOffset, float yOffset)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch -= yOffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	updateCameraVectors();
}

void Camera::setRotation(float yawAngle, float pitchAngle)
{
	yaw = yawAngle;
	pitch = pitchAngle;

	// Clamp pitch
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}