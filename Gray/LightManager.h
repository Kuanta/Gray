#pragma once
#define MAX_NUM_LIGHTS 100
#include "grLight.h"
#include "ElementManager.h"
#include "UniformBuffer.h"

//TODO: Creating structs at each draw calls can be costly. Find an alternate solution
struct GpuLight {
	//Represenation of a light in the shader. GrLights must be transformed into these structs so that they can be sent to uniform buffers.
	glm::vec4 color;
	glm::vec4 position;
	glm::vec4 direction;
	int type;
	float intensity;
	float constant;
	float linear;
	float quadratic;
	float dummy4;
	float dummy5;
	float dummy6;
};

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
	UniformBuffer* lightsBuffer = nullptr;

private:
	int numberOfLights = 0; //Number of current lights in the scene
	void updateBuffer(); //Update buffer only lights change
	GpuLight lightStructs[MAX_NUM_LIGHTS];
};

