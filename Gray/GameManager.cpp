#include "GameManager.h"

//Input Methods

bool GameManager::init(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	this->window = glfwCreateWindow(width, height, "Gray", NULL, NULL);
	if (window == NULL)
	{
		return false;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}
	glViewport(0, 0, width, height);

	//Set input callbacks
	glfwSetKeyCallback(this->window, this->keyCallback);
	glfwSetCharCallback(this->window, character_callback);
	glfwSetCharModsCallback(this->window, charmods_callback);
	glfwSetCursorEnterCallback(this->window, cursor_enter_callback);
	glfwSetCursorPosCallback(this->window, cursor_position_callback);
	glfwSetMouseButtonCallback(this->window, mouse_button_callback);
	glfwSetDropCallback(this->window, drop_callback);
	glfwSetScrollCallback(this->window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//Set window pointer to this
	glfwSetWindowUserPointer(window, this);
	

	//Define Shaders
	this->shaders.defaultShader = new Shader("Shaders/simple_vertex.txt", "Shaders/simple_fragment.txt");

	return true;
}
void GameManager::update()
{
	
	glEnable(GL_DEPTH_TEST);
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	glClearColor(0.3f, 0.8f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->currentScene->update(this->window, deltaTime);
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void GameManager::changeScene(Scene * newScene)
{
	this->currentScene->cleanup();
	this->currentScene = newScene;
}

GameManager::GameManager(int width, int height)
{
	this->init(width, height);
}

GameManager::GameManager()
{
}
void GameManager::cleanup()
{
	cout << "Clearing GameManager..." << endl;
	currentScene->cleanup();
}


GameManager::~GameManager()
{
}
void GameManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Since this is a static function, 'this' keyword can't be used in this context
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		gm->quitting = true;
		gm->cleanup();
		glfwSetWindowShouldClose(window, true);
	}

	if (!gm->quitting)
	{
		//Still checking for input while quitting the game causes null pointer errors
		gm->currentScene->keyInput(key, scancode, action, mods);
	}

}

void GameManager::character_callback(GLFWwindow * window, unsigned int codepoint)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	gm->currentScene->character_callback(window, codepoint);
}

void GameManager::charmods_callback(GLFWwindow * window, unsigned int codepoint, int mods)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	gm->currentScene->charmods_callback(window, codepoint, mods);
}

void GameManager::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	gm->currentScene->cursor_position_callback(window, xpos, ypos);
}

void GameManager::cursor_enter_callback(GLFWwindow * window, int entered)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	gm->currentScene->cursor_enter_callback(window, entered);
}

void GameManager::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	gm->currentScene->mouse_button_callback(window, button, action, mods);
}

void GameManager::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	gm->currentScene->scroll_callback(window, xoffset, yoffset);
}

void GameManager::drop_callback(GLFWwindow * window, int count, const char ** paths)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	gm->currentScene->drop_callback(window, count, paths);
}

void GameManager::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	gm->currentScene->framebuffer_size_callback(window, width, height);
}
