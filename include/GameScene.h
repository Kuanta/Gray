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
#include "Plane.h"
#include "grMesh.h"

class GameScene : public Scene
{
public:
	GameScene(GameManager* gm);
	~GameScene();

	// Scene arac�l���yla devral�nd�
	virtual void draw(double deltaTime);
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

	//For demo purposes
	void initFbo();

private:
	//Debug
	float testTime=0;
	Object* player;
	GrAnimManager* animMan=nullptr;

	//For demo purposes
	unsigned int fbo;
	unsigned int fboTexture;
	unsigned int rbo;
	Object* fboCanvas;
	void drawFbo();

};
