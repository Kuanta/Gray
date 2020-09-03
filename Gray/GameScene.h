#pragma once
#include "GameManager.h"
#include "Scene.h"
#include "Camera.h"
#include "ElementManager.h"
#include "Object.h"
#include "grLight.h"
#include "grDirectionalLight.h"
#include "grPointLight.h"  
#include "Model.h"
#include "GrAnimManager.h"

class GameScene : public Scene
{
public:
	GameScene(GameManager* gm);
	~GameScene();
	
	ElementManager<Object*> em;
	ElementManager<GrLight*> lights;

	GrLight ambientLight;
	grDirectionalLight dirLight;
	grPointLight pointLight;
	// Scene aracýlýðýyla devralýndý
	virtual void fixedUpdate() override;
	virtual void update(GLFWwindow * window, double deltaTime) override;
	virtual void draw(double deltaTime) override;
	virtual void keyInput(int key, int scancode, int action, int mods) override;
	virtual void character_callback(GLFWwindow * window, unsigned int codepoint) override;
	virtual void charmods_callback(GLFWwindow * window, unsigned int codepoint, int mods) override;
	virtual void cursor_position_callback(GLFWwindow * window, double xpos, double ypos) override;
	virtual void cursor_enter_callback(GLFWwindow * window, int entered) override;
	virtual void mouse_button_callback(GLFWwindow * window, int button, int action, int mods) override;
	virtual void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) override;
	virtual void drop_callback(GLFWwindow * window, int count, const char ** paths) override;
	virtual void framebuffer_size_callback(GLFWwindow * window, int width, int height) override;
	virtual void cleanup() override;

private:
	//Debug
	float testTime=0;
	Object* player;
	GrAnimManager* animMan=nullptr;
};

