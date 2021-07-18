#include "GameManager.h"

//Input Methods

Shader* GameManager::getShader(SHADER_TYPE shaderType)
{
	return this->shaderMan.getShaderByType(shaderType);
}

bool GameManager::init(int width, int height)
{
	this->currentScene = nullptr;
	//GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RED_BITS, 16);
	glfwWindowHint(GLFW_GREEN_BITS, 16);
	glfwWindowHint(GLFW_BLUE_BITS, 16);
	glfwWindowHint(GLFW_ALPHA_BITS, 16);
	glfwWindowHint(GLFW_STENCIL_BITS, 16);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	this->window = glfwCreateWindow(width, height, "Gray", NULL, NULL);
	this->windowWidth = width; this->windowHeight = height;
	if (window == NULL)
	{
		return false;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  //Load GL functio�ns
	{
		return false;
	}

	//ALternatively
	/*if (!gladLoadGL()) {
		std::cerr << "Unable to load OpenGL functions!" << std::endl;
		exit(EXIT_FAILURE);
	}*/
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
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);

	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Set window pointer to this
	glfwSetWindowUserPointer(window, this);

	//Shader Manager requires a pointer to the game manager so that it can bind uniform buffer whenever a new shader is initialized
	this->shaderMan.gm = this;
	this->assetMan.gm = this;

	//Initialize Buffers
	this->matricesBuffer = new UniformBuffer(16*9, "Matrices", 0);
	this->lightsBuffer = new UniformBuffer(MAX_NUM_LIGHTS*144 + 4, "Lights", 1);

	//OPENGL
	glEnable(GL_DEPTH_TEST);  //Enable depth testing
	glDepthFunc(GL_LESS);  //Type of depth chechking

	return true;
}
void GameManager::update()
{
	currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	this->currentScene->earlyUpdate(deltaTime);
	this->currentScene->update(this->window, deltaTime);
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void GameManager::changeScene(Scene * newScene)
{
	if (this->currentScene != nullptr)
	{
		this->currentScene->cleanup();
	}
	this->currentScene = newScene;
	this->currentScene->matricesBuffer = this->matricesBuffer;
	this->currentScene->lm.lightsBuffer = this->lightsBuffer;
	this->currentScene->init();
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
		if(gm->currentScene->keyInputCallback != nullptr)
		{
			gm->currentScene->keyInputCallback(key, scancode, action, mods);
		}
	}

}

void GameManager::character_callback(GLFWwindow * window, unsigned int codepoint)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if(gm->currentScene->characterCallback != nullptr)
	{
		gm->currentScene->characterCallback(window, codepoint);
	}
}

void GameManager::charmods_callback(GLFWwindow * window, unsigned int codepoint, int mods)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if(gm->currentScene->charmodsCallback != nullptr)
	{
		gm->currentScene->charmodsCallback(window, codepoint, mods);
	}
}

void GameManager::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if(gm->currentScene->cursorPositionCallback != nullptr)
	{
		gm->currentScene->cursorPositionCallback(window, xpos, ypos);
	}
}

void GameManager::cursor_enter_callback(GLFWwindow * window, int entered)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if(gm->currentScene->cursorEnterCallback != nullptr)
	{
		gm->currentScene->cursorEnterCallback(window, entered);
	}
}

void GameManager::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if(gm->currentScene->mouseButtonCallback != nullptr)
	{
		gm->currentScene->mouseButtonCallback(window, button, action, mods);
	}
}

void GameManager::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if(gm->currentScene->scrollCallback != nullptr) 
	{
		gm->currentScene->scrollCallback(window, xoffset, yoffset);
	}
}

void GameManager::drop_callback(GLFWwindow * window, int count, const char ** paths)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if(gm->currentScene->dropCallback != nullptr)
	{
		gm->currentScene->dropCallback(window, count, paths);
	}
}

void GameManager::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	GameManager* gm = (GameManager*)glfwGetWindowUserPointer(window); //Retrieve the user pointer
	if(gm->currentScene->frameBufferSizeCallback != nullptr){gm->currentScene->frameBufferSizeCallback(window, width, height);}
}
