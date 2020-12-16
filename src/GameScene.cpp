#include "GameScene.h"

GrAnimation* walk=nullptr;
GrAnimation* stand=nullptr;

GameScene::GameScene(GameManager* gm) : Scene(gm)
{
	this->camera = Camera(75, 1280 / 800, 0.1f, 100000000.0f);

	this->camera.setPositionY(8);
	this->camera.setTarget(glm::vec3(0.0f, 0.0f, -1.0f));
	Model* model = new Model();
	player = model->loadModel(gm, "assets/arthas/arthas.fbx");
	cout<<"Created Scene"<<endl;

	player->setShader(gm->getShader(SHADER_TYPE::DEFAULT_SHADER));
	model->importAnimations("assets/arthas/arthas_Walk_1.fbx");
	model->importAnimations("assets/arthas/arthas_Stand_0.fbx");
	model->importAnimations("assets/arthas/arthas_EmoteDance_132.fbx"); //"Attack1H [82]
	model->importAnimations("assets/arthas/arthas_EmoteKiss_49.fbx");
	this->animMan = (GrAnimManager*)player->getComponentByType(ComponentType::ANIMATION_MANAGER);
	cout<<"Created Scene"<<endl;
	stand = this->animMan->getAnimation("Stand [0]");
	walk = this->animMan->getAnimation("Walk [1]");
	//this->animMan->addToActive("EmoteKiss [49]", 1.0f, true);
	this->animMan->addToActive(stand, 1.0f, true);
	this->animMan->addToActive(walk, 0.0f, true);
	if (player != nullptr)
	{
		player->setScale(glm::vec3(0.5, 0.5, 0.5));
		player->setPositionY(0);
		player->setPositionZ(-30);
		this->em.addElement(player);
		player->gm = gm;
	}
	for(int i=0;i<10;i++)
	{
		Object* clone = player->clone();
		clone->setPosition(i*1.5, i*1.5, i*1.5);
		this->add(clone);
		clone->gm = this->gm;
	}
}


GameScene::~GameScene()
{
}

void GameScene::draw(double deltaTime)
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo); //Bind the default fb just in case 
	Scene::draw(deltaTime);

	this->drawFbo();

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
		this->animMan->addToActive("EmoteDance [132]",1.0f, false);
		this->animMan->startTransition(0.1f);
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		this->animMan->addToActive("EmoteKiss [49]", 1.0f, false);
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

void GameScene::initFbo()
{
	/*
	*	This function is for demo purposes. It initializes a framebuffer alongside with a texture.
	*/
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Bind the texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fboTexture, 0);

	//Assign a render object to frame buffer for stencil and depth testing
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 800);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "ERROR: Framebuffer is not complete" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->fboCanvas = new Object();
	GrMesh* fboCanvasMesh;
	Plane* pg = new Plane(2.0f, 2.0f);
	Material* mat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true, 1);
	fboCanvasMesh = new GrMesh(pg, mat);
	this->fboCanvas->addComponent(fboCanvasMesh);
}

void GameScene::drawFbo()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	

	Shader* shader = this->gm->shaderMan.getShaderByType(SHADER_TYPE::FBO_SHADER);
	shader->use();
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->fboTexture);

	//glReadBuffer(GL_COLOR_ATTACHMENT0);
	//unsigned int pixels[4];
	//glReadPixels(100,100, 1200, 800, GL_RGBA, GL_UNSIGNED_INT,pixels);
	//cout << pixels[0]<< " - "<<pixels[1]<< endl;
	this->fboCanvas->draw(shader);
}
