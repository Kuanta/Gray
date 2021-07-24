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
	glm::vec4 color; //16
	glm::vec4 position; // 16
	glm::vec4 direction; //16
	int type; //4
	float intensity; //4
	float constant; //4
	float linear; //4
	float quadratic;
	int castShadow;
	float far_plane;
	float attenuationFactor;
	int softnessFactor;
	float dummy1;
	float dummy2;
	float dummy3;
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
	int shadowDepth = 4096;
	int shadowHeight = 4096;
	int pointShadowDepth = 2048;
	int pointShadowHeight = 2048;

	int numberOfLights = 0; //Number of current lights in the scene
	void updateBuffer(); //Update buffer only lights change
	GpuLight lightStructs[MAX_NUM_LIGHTS];
	unsigned int depthMap;  //Texture array
	unsigned int depthCubeMap; //For Point Lights

	unsigned int depthMapStatic; //For static objects
	unsigned int depthCubeMapStatic; //For static objects

	unsigned int depthMapFBO; //Framebuffer for dynamicdirectionals
	unsigned int depthMapFBOCube;
	unsigned int depthMapFBOStatic; //Framebuffer for dynamicdirectionals
	unsigned int depthMapFBOCubeStatic;

	unsigned int finalShadowTexture; //Draw all the shadows here
	unsigned int createDepthMap(unsigned int fbo, unsigned int depthMap);
	unsigned int createCubeMap(unsigned int fbo, unsigned int cubeMap);
	void prepareDrawing(GrLight *light, unsigned int fbo, bool isStatic, int index); //Use before drawing elements

	int MAX_DIRECT_LIGHT = 3;
	int MAX_POINT_LIGHT = 30;
	int test = 10;
};

#endif //LIGHTMANAGER_H