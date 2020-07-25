#pragma once
#include <map>
#include "Component.h"
#include "GrAnimation.h"
#include "containers.h"

class GrAnimManager : public Component
{
public:
	GrAnimManager();
	~GrAnimManager();

	// Inherited via Component
	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void cleanup() override;

	void addAnimation(string name, GrAnimation* anim);
	void addAnimation(GrAnimation* anim);
	void addAnimations(vector<GrAnimation*> anims);
	void changeAnimation(string animName, float transitionTime=1.0f, bool loop=false);
	void changeAnimation(int animId, float transitionTime=1.0f, bool loop=false);
	void changeAnimation(GrAnimation* newAnim, float transitionTime=1.0f, bool loop=false);
	GrAnimation* getAnimation(string name);
	void resetAnimation();
	void resetToIdle();
	void resetToBind();
	void transitionTo(GrAnimation* anim, float transitionTime=1.0f);
	map<string, GrAnimation*> animations;

	void mergeAnimations(GrAnimManager* animMan);
	void addToActive(string name, float weight, bool loop=false);
	void addToActive(GrAnimation* anim, float weight, bool loop=false);
	void static clearAnimCallback(GrAnimation* anim);

private:
	GrAnimation* currentAnimation = nullptr;
	GrAnimation* secondaryAnimation = nullptr;
	linked::LinkedList<GrAnimation*> activeAnimatons;
	float animationFactor = 1.0f; //Used in blending
	//Transition variables
	bool transitioning;
	GrAnimation* targetAnimation = nullptr;
	float transitionTime;
	float totalWeights = 0; //TOtal weights of active animations. Used for normalization
};

