#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Camera;

class Controls
{
public:
	Controls();
	~Controls();

	//A Camera is needed to control
	Camera* camera;
	float lastX;
	float lastY;
	float deltaX; //Coordinate change of the cursor in x direction
	float deltaY; //Coordinate change of the cursor in y direction
	bool firstMove = true;

	//Input callbacks
	virtual void keyInput(int key, int scancode, int action, int mods);
	virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	virtual void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	virtual void cursor_enter_callback(GLFWwindow* window, int entered);
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	virtual void update(float deltaTime) = 0;
};

