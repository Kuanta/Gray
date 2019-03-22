#include "Camera.h"



Camera::Camera(float fov, float aspectRatio, float near, float far)
{
	this->projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
}


Camera::~Camera()
{
}

glm::vec3 Camera::getFrontVector()
{	
	return glm::normalize(glm::vec3(this->target.x - this->position.x, 0, this->target.z - this->position.z));
}

glm::vec3 Camera::getRightVector()
{
	return glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->getFrontVector()));
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->position, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::updateDirections()
{
	this->target.x = cos(glm::radians(this->rotation.x))*cos(glm::radians(this->rotation.y));
	this->target.y = sin(glm::radians(this->rotation.x));
	this->target.z = cos(glm::radians(this->rotation.x))*sin(glm::radians(this->rotation.y));
	this->right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),this->getFrontVector()));
	this->up = glm::normalize(glm::cross(this->target, this->right));
}

void Camera::update(float deltaTime)
{	
	this->updateDirections();
	if (this->controls != nullptr)
	{
		this->controls->update(deltaTime);
	}
}
