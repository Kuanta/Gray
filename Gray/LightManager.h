#pragma once
#define LIGHT_BUFFER_SIZE 152
#include "grLight.h"
#include "ElementManager.h"

class LightManager
{
public:
	//Methods
	LightManager();
	~LightManager();
	void init();
	void add(GrLight* light);
	void remove(GrLight* light);
	void update(float delta);
	void draw();  //Draw function doesn't require a Shader because the lights will be send to uniform buffer
	void clean();

	//Members
	ElementManager<GrLight*> lights;
private:
	int numberOfLights = 0; //Number of current lights in the scene
	unsigned int lightsBuffer;
	void updateBuffer(); //Update buffer only lights change
};

