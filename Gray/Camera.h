#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"
#include "Controls.h"

class Camera : public Object
{
public:
	Camera() {};
	Camera(float fov, float aspectRatio, float near, float far);
	~Camera();
	Controls* controls = nullptr;
	glm::mat4 projection;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 left;
	float getRoll() { return this->getRotation().z; }
	float getPitch() { return this->getRotation().x; }
	float getYaw() { return this->getRotation().y; }
	glm::vec3 getFrontVector();
	glm::vec3 getRightVector();
	glm::mat4 getViewMatrix();
	void updateDirections();
	virtual void update(float deltaTime) override;
private:

};

