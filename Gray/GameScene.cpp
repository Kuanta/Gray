#include "GameScene.h"



GameScene::GameScene(GameManager* gm) : Scene(gm)
{
	this->camera = Camera(75, 800/600, 0.001f, 10000.0f);
	this->ambientLight = grLight(glm::vec3(0.3, 0.3, 0.3), 0.4);
	this->dirLight = grDirectionalLight(glm::vec3(0.4, 0.4, 0.4), 0.3,glm::vec3(0.0f,-1.0f, 0.0f));
	this->pointLight = grPointLight(glm::vec3(1.0f, 0.4f, 0.8f), 0.8f, 1.0f, 0.014f*0.01, 0.07f*0.01);
	this->pointLight.setPosition(5, 15, -10);
	this->camera.position.z = 8;
	this->camera.setTarget(glm::vec3(0.0f, 0.0f, -1.0f));
}


GameScene::~GameScene()
{
}

void GameScene::fixedUpdate()
{
}

void GameScene::update(GLFWwindow * window, double deltaTime)
{
	this->camera.controls->update(deltaTime);

	for (vector<Object*>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->update(deltaTime);
		}
		this->pointLight.position = this->camera.position;
	}
	this->camera.getViewMatrix();

	this->draw(deltaTime);
	this->em.clearElements();

}

void GameScene::draw(double deltaTime)
{
	//Draw lights
	this->gm->shaders.defaultShader->setVec3("eyePosition", this->camera.position);
	this->ambientLight.useLight(this->gm->shaders.defaultShader);
	this->dirLight.useLight(this->gm->shaders.defaultShader);
	this->pointLight.useLight(this->gm->shaders.defaultShader);
	for (vector<Object*>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->draw();
		}

	}
}

void GameScene::keyInput(int key, int scancode, int action, int mods)
{
	if (this->camera.controls != nullptr)
	{
		this->camera.controls->keyInput(key, scancode, action, mods);
	}
}

void GameScene::character_callback(GLFWwindow * window, unsigned int codepoint)
{
}

void GameScene::charmods_callback(GLFWwindow * window, unsigned int codepoint, int mods)
{
}

void GameScene::cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (this->camera.controls != nullptr)
	{
		this->camera.controls->cursor_position_callback(window, xpos, ypos);
	}
}

void GameScene::cursor_enter_callback(GLFWwindow * window, int entered)
{
}

void GameScene::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (this->camera.controls != nullptr)
	{
		this->camera.controls->mouse_button_callback(window, button, action, mods);
	}
}

void GameScene::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	if (this->camera.controls != nullptr)
	{
		this->camera.controls->scroll_callback(window, xoffset, yoffset);
	}
}

void GameScene::drop_callback(GLFWwindow * window, int count, const char ** paths)
{
}

void GameScene::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
}

void GameScene::cleanup()
{
}
