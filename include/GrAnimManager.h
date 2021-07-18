#pragma once
#include <map>
#include "Components/Component.h"
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

	GrAnimation* getAnimation(string name);
	void resetAnimation();
	void startTransition(float transitionTime=0.1f);
	void transitionTo(GrAnimation* anim, float transitionTime=1.0f);
	map<string, GrAnimation*> animations;

	void mergeAnimations(GrAnimManager* animMan);
	void setActiveAnimation(GrAnimation* anim, void* (*callback)()=nullptr);
	void addToActive(string name, float weight, bool loop=false, void* (*callback)()=nullptr);
	void addToActive(GrAnimation* anim, float weight, bool loop=false, void* (*callback)() = nullptr);
	void static clearAnimCallback(GrAnimation* anim);
	

private:
	GrAnimation* currentAnimation = nullptr;
	GrAnimation* secondaryAnimation = nullptr;
	linked::LinkedList<GrAnimation*> activeAnimations;
	float animationFactor = 1.0f; //Used in blending
	//Transition variables
	bool transitioning=false;
	GrAnimation* targetAnimation = nullptr;
	float transitionTime = 0.0f;
	float totalWeights = 0;
	// Inherited via Component
	virtual Component* clone() override;
	//TOtal weights of active animations. Used for normalization
};

