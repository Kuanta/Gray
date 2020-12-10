#pragma once

#include <iostream>
#include "Object.h"
#include "Camera.h"
#include "LightManager.h"
#include "UniformBuffer.h"
#include <GLFW/glfw3.h>



class GameManager;
using namespace std;
class Scene
{
public:
	GameManager* gm; //To access changeScene method
	GLFWwindow* window;
	Camera camera;
	ElementManager<Object*> em;
	LightManager lm;
	UniformBuffer* matricesBuffer = nullptr;

	void add(Object* object);
	void add(GrLight* light);
	void remove(Object* object);
	void remove(GrLight* light);
	virtual void fixedUpdate();
	virtual void update(GLFWwindow* window, double deltaTime);
	virtual void draw(double deltaTime);
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
