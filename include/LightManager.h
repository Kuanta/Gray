#pragma once
#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#define MAX_NUM_LIGHTS 100
#include <glm/gtx/string_cast.hpp>
#include "Lights/grLight.h"
#include "ElementManager.h"
#include "UniformBuffer.h"
#include "Shader.h"

class Scene;
class GameManager;

//TODO: Creating structs at each draw calls can be costly. Find an alternate solution
struct GpuLight {
	//Represenation of a light in the shader. GrLights must be transformed into these structs so that they can be sent to uniform buffers.
	// glm::vec4 color;  //16
	// glm::vec4 position; //16
	// glm::vec4 direction;  //16
	// glm::mat4 transformMat; //4*16
	// int type; 
	// float intensity;
	// float constant;
	// float linear;
	// float quadratic;
	// bool castShadow;
	// float dummmy1;
	glm::mat4 transformMat; //4*16
	glm::vec4 color;
	glm::vec4 position;
	glm::vec4 direction;
	int type;
	float intensity;
	float constant;
	float linear;
	float quadratic;
	int castShadow;
	float dummy5;
	float dummy6;
};

class LightManager
{
public:
	//Methods
	LightManager();
	LightManager(GameManager* gm, Scene* scene);
	~LightManager();
	void init();
	void add(GrLight* light);
	void remove(GrLight* light);
	void update(float delta);
	void draw();  //Draw function doesn't require a Shader because the lights will be send to uniform buffer
	void clean();
	void activateShadowMap(int layer);

	//Members
	GameManager* gm;
	Scene* scene;
	ElementManager<GrLight*> lights;
	UniformBuffer* lightsBuffer = nullptr;

private:
	int numberOfLights = 0; //Number of current lights in the scene
	void updateBuffer(); //Update buffer only lights change
	GpuLight lightStructs[MAX_NUM_LIGHTS];
	unsigned int shadowMaps;  //Texture array
	unsigned int depthMapFBO;
};

#endif //LIGHTMANAGER_H