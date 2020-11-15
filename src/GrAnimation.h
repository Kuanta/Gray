#pragma once
#include <assimp/scene.h>
#include <map>
#include <math.h>
#include "GrSkeleton.h"

class GrAnimManager;
struct GrAnimFrame
{
	/*
		Simple structure that holds pointers to a single frame. 
	*/
	glm::vec3* posKey;
	glm::vec3* scaleKey;
	glm::quat* rotKey;
};
class GrAnimationNode
{
public:
	GrAnimationNode(aiNodeAnim* node);
	vector<glm::vec3> posKeys;
	vector<glm::vec3> scaleKeys;
	vector<glm::quat> rotKeys;
	string name;
	int keyNum;
	vector<float> times;
	GrAnimFrame getAnimFrame(float relTime, float ticksPerSecond, int direction);
	glm::vec3 currPos;
	glm::vec3 currRot;
	glm::vec3 currScale;
};

class GrAnimation
{
public:
	GrAnimation();
	GrAnimation(string name, double duration, double ticksPerSecond);
	~GrAnimation();

	void update(float deltaTime, GrSkeleton* skeleton, bool transition, float transitionFactor = 0.0f);
	void fadeTo(float targetWeight, float speed);
	void transition(float transitionFactor, GrSkeleton* skeleton);
	void resetAnimation();
	string name;
	GrSkeleton* skeleton;
	GrAnimManager* animManager;
	map<string, GrAnimationNode*> animNodes;
	double duration;
	double ticksPerSecond;
	float speed = 1.0f;
	int direction = 1; //-1 means the animation goes backwards
	float weight = 1.0f; //Weight of the animation during blending

	glm::vec3* currPos;
	glm::quat* currRot;
	glm::vec3* currScale;

	bool active = false; //Will be set to true when added to activeAnimations. If set to false again will be removed from active animations
	bool fading = false;
	bool loop = true; //If loop, will continue after completing a cycle
	
private:
	float animTime = 0.0f;
	float targetWeight;
	float fadeSpeed;
};

