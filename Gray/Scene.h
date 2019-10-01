#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"


class GameManager;
using namespace std;
class Scene
{
public:
	GameManager* gm; //To access changeScene method
	GLFWwindow* window;
	Camera camera;

	virtual void fixedUpdate()=0;
	virtual void update(GLFWwindow* window, double deltaTime)=0;
	virtual void draw(double deltaTime)=0;
	virtual void keyInput(int key, int scancode, int action, int mods)=0;
	virtual void character_callback(GLFWwindow* window, unsigned int codepoint)=0;
	virtual void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods)=0;
	virtual void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)=0;
	virtual void cursor_enter_callback(GLFWwindow* window, int entered)=0;
	virtual void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)=0;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)=0;
	virtual void drop_callback(GLFWwindow* window, int count, const char** paths)=0;
	virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height)=0;
	virtual void cleanup() = 0;
	
	Scene(GameManager* gm);
	~Scene();

};

