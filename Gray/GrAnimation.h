#pragma once
#include <assimp\scene.h>
#include <map>
#include <math.h>
#include "GrSkeleton.h"

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
};

class GrAnimation
{
public:
	GrAnimation();
	GrAnimation(string name, double duration, double ticksPerSecond);
	~GrAnimation();

	void update(float deltaTime, GrSkeleton* skeleton);
	void transition(float transitionFactor, GrSkeleton* skeleton);
	void resetAnimation();
	string name;
	map<string, GrAnimationNode*> animNodes;
	double duration;
	double ticksPerSecond;
	float speed = 1.0f;
	int direction = 1; //-1 means the animation goes backwards

private:
	float animTime = 0.0f;
};

