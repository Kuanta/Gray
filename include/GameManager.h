#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#define MAX_NUM_LIGHTS 100

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Scene.h"
#include "ShaderManager.h"
#include "AssetManager.h"
#include "Shader.h"
#include "UniformBuffer.h"

using namespace std;

struct Shaders
{
	Shader* defaultShader;
	Shader* pbrShader;
};

class GameManager
{
public:
	GLFWwindow* window;
	Scene* currentScene;
	ShaderManager shaderMan;
	AssetManager assetMan;
	UniformBuffer* matricesBuffer = nullptr;
	UniformBuffer* lightsBuffer = nullptr;
	int windowWidth;
	int windowHeight;

	Shader* getShader(SHADER_TYPE shaderType);
	bool init(int width, int height);
	void update();
	void changeScene(Scene* newScene);
	void cleanup();

	//Input Methods
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void character_callback(GLFWwindow* window, unsigned int codepoint);
	static void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void cursor_enter_callback(GLFWwindow* window, int entered);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void drop_callback(GLFWwindow* window, int count, const char** paths);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	GameManager(int width, int height);
	GameManager();
	~GameManager();

private:
	double currentFrame;
	double lastFrame = 0;
	double deltaTime;
	bool quitting = false;
};

#endif