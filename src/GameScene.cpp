#include "GameScene.h"

GrAnimation* walk=nullptr;
GrAnimation* stand=nullptr;

GameScene::GameScene(GameManager* gm) : Scene(gm)
{
	this->camera = Camera(75, 16/9, 0.1f, 100000000.0f);  
	this->camera.setPositionY(8);
	this->camera.setTarget(glm::vec3(0.0f, 0.0f, -1.0f));
	Model* model = new Model();
	player = model->loadModel(gm, "./assets/arthas/arthas.fbx");
	player->setShader(gm->getShader(SHADER_TYPE::PBR_SHADER));
	model->importAnimations("./assets/arthas/arthas_Walk_1.fbx");
	model->importAnimations("./assets/arthas/arthas_Stand_0.fbx");
	model->importAnimations("./assets/arthas/arthas_Attack1H_82.fbx"); //"Attack1H [82]
	model->importAnimations("./assets/arthas/arthas_EmoteKiss_49.fbx");
	this->animMan = (GrAnimManager*)player->getComponentByType(ComponentType::ANIMATION_MANAGER);
	stand = this->animMan->getAnimation("Stand [0]");
	walk = this->animMan->getAnimation("Walk [1]");
	//this->animMan->addToActive("EmoteKiss [49]", 1.0f, true);
	this->animMan->addToActive(stand, 1.0f, true);
	this->animMan->addToActive(walk, 0.0f, true);
	if (player != nullptr)
	{
		player->setScale(glm::vec3(0.25, 0.25, 0.25));
		player->setRotationY(-90);
		player->setPositionY(0);
		player->setPositionZ(10);
		this->em.addElement(player);
		player->gm = gm;
	}
	//TODO: If this code doesn't slow down things, then it means that we have solved skeletal animation optimization issue
	// for(int r=0;r<10;r++)
	// {
	// 	for(int i=0; i<5;i++)
	// 	{
	// 		Object* clone = player->clone();
	// 		clone->setPosition(r*25*cos(i*3.14*30/180), 3, r*30*sin(i*3.14*30/180));
	// 		clone->setScale(0.2f, 0.2f, 0.2f);
	// 		clone->setRotationY(-90);
	// 		clone->gm = this->gm;
	// 		this->add(clone);
	// 	}
	// }
}


GameScene::~GameScene()
{
}

void GameScene::draw(double deltaTime)
{
	Scene::draw(deltaTime);
}

void GameScene::keyInput(int key, int scancode, int action, int mods)
{
	if (this->camera.controls != nullptr)
	{
		this->camera.controls->keyInput(key, scancode, action, mods);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		walk->weight = 1.0f;
		stand->weight = 0.0f;
		animMan->startTransition(0.1f);
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		walk->weight = 0.0f;
		stand->weight = 1.0f;
		animMan->startTransition(0.1f);
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		this->animMan->addToActive("Attack1H [82]",1.0f, false);
		this->animMan->startTransition(0.1f);
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		this->animMan->addToActive("EmoteKiss [49]", 100.0f, false);
		this->animMan->startTransition(0.1f);
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