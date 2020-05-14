#include "GrAnimManager.h"

GrAnimManager::GrAnimManager()
{
	this->type = ComponentType::ANIMATION_MANAGER;
}

GrAnimManager::~GrAnimManager()
{
}

void GrAnimManager::start()
{
}

void GrAnimManager::update(float deltaTime)
{
	GrSkeleton* skeleton = (GrSkeleton*)this->object->getComponentByType(ComponentType::SKELETON);
	if (!this->transitioning)
	{
		if (this->currentAnimation)
		{
			
			this->currentAnimation->update(deltaTime, skeleton);
		}
	}
	else
	{
		float factor;
		if (deltaTime > this->transitionTime)
		{
			factor = 1.0f;
			this->transitioning = false;
			this->currentAnimation = this->targetAnimation;
			this->targetAnimation = nullptr;
		}
		else
		{
			factor = deltaTime / this->transitionTime;
			this->targetAnimation->transition(factor, skeleton);
		}
		this->transitionTime -= deltaTime;
	}
	
}

void GrAnimManager::cleanup()
{

}

void GrAnimManager::addAnimation(string name, GrAnimation* anim)
{
	this->animations.insert(std::pair<string, GrAnimation*>(name, anim));
}

void GrAnimManager::addAnimation(GrAnimation* anim)
{
	this->animations.insert(std::pair<string, GrAnimation*>(anim->name, anim));
}

void GrAnimManager::addAnimations(vector<GrAnimation*> anims)
{
	for (vector<GrAnimation*>::iterator it = anims.begin(); it != anims.end(); it++)
	{
		if ((*it) != nullptr)
		{
			this->addAnimation(*it);
		}
	}
}

void GrAnimManager::changeAnimation(string animName)
{
	this->changeAnimation(this->animations.at(animName));
}

void GrAnimManager::changeAnimation(int animId)
{
}

void GrAnimManager::changeAnimation(GrAnimation* newAnim)
{
	if (!this->transitioning) //Don't change animation while transitioning
	{
		this->transitionTo(newAnim);
	}
	
}

void GrAnimManager::resetAnimation()
{

}

void GrAnimManager::transitionTo(GrAnimation* anim, float transitionTime)
{
	this->transitionTime = transitionTime; //TODO: Set this as optional
	this->targetAnimation = anim;
	this->targetAnimation->resetAnimation();
	this->transitioning = true;
}

void GrAnimManager::mergeAnimations(GrAnimManager* animMan)
{
	this->animations.insert(animMan->animations.begin(), animMan->animations.end());
}
