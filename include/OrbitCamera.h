#pragma once
#include <math.h>
#include "Camera.h"
#include "Controls.h"

#define PI 3.14159265

class OrbitCamera : public Controls
{
public:
	OrbitCamera(Camera* camera);
	~OrbitCamera();

	Camera* camera;

	virtual void update(float deltaTime) override;
	virtual void keyInput(int key, int scancode, int action, int mods) override;
	virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) override;
	virtual void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) override;
	virtual void cursor_enter_callback(GLFWwindow* window, int entered) override;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;

private:
	float orbitSens = 5.0f;
	float panSens = 1.5f;
	float radiusSens = 100.0f;

	float orbitTimer = 0.0f;
	float panTimer = 0.0f;
	float radiusTimer = 0.0f;
	float minRadius = 0.01f;
	float maxRadius = 1000.0f;
	float maxDragTime = 100.0f;
	float yaw = 0;
	float pitch = 0;
	float radius = 40;
	float yawAcc = 0;
	float yawVel = 0;
	float pitchAcc = 0;
	float pitchVel = 0;
	float radiusAcc = 0;
	float radiusVel = 0;
	float panRightAcc = 0;
	float panRightVel = 0;
	float panFrontAcc = 0;
	float panFrontVel = 0;
	float damping = 0.8;

	//Buttons pressed?
	bool leftMousePressed = false;
	bool rightMousePressed = false;
	bool middleMousePressed = false;
	glm::vec2 leftPressedPos;
	glm::vec2 rightPressedPos;
	glm::vec2 middleMousePressedPos;
};

