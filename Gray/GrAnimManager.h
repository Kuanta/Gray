#pragma once
#include <map>
#include "Component.h"
#include "GrAnimation.h"

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
	void changeAnimation(string animName, float transitionTime=1.0f);
	void changeAnimation(int animId, float transitionTime);
	void changeAnimation(GrAnimation* newAnim, float transitionTime=1.0f);
	void resetAnimation();
	void resetToIdle();
	void resetToBind();
	void transitionTo(GrAnimation* anim, float transitionTime=1.0f);
	map<string, GrAnimation*> animations;

	void mergeAnimations(GrAnimManager* animMan);

private:
	GrAnimation* currentAnimation = nullptr;
	GrAnimation* secondaryAnimation = nullptr;
	float animationFactor = 1.0f; //Used in blending
	GrAnimation* idleAnimation = nullptr;
	//Transition variables
	bool transitioning;
	GrAnimation* targetAnimation = nullptr;
	float transitionTime;
};

