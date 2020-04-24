#pragma once
#include <assimp\scene.h>
#include <map>
#include <math.h>
#include "Component.h"
#include "GrSkeleton.h"

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


	void updateBone(GrBone* bone, float animTime, float totalDuration);
};

class GrAnimation : public Component
{
public:
	GrAnimation();
	GrAnimation(string name, double duration, double ticksPerSecond);
	~GrAnimation();

	// Component aracýlýðýyla devralýndý
	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void cleanup() override;
	string name;
	double duration;
	double ticksPerSecond;
	map<string, GrAnimationNode*> animNodes;
private:
	float animTime = 0.0f;
};

