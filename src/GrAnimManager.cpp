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
	float factor = 0.0f;
	if (this->transitioning)
	{
		factor = this->transitionClock / this->transitionTime;
		this->transitionClock += deltaTime;
	}
	if(this->activeAnimation != nullptr)
	{
		this->activeAnimation->update(deltaTime, skeleton, this->transitioning, factor);
	}
	if (this->transitioning && this->transitionClock >= this->transitionTime)
	{
		this->transitioning = false;
	}
}

void GrAnimManager::cleanup()
{

}
void GrAnimManager::setActiveAnimation(GrAnimation *anim, void *(*callback)())
{
	anim->resetAnimation();
	this->activeAnimation = anim;
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
			(*it)->animManager = this;
		}
	}
}

GrAnimation* GrAnimManager::getAnimation(string name)
{
	if (this->animations.find(name) != this->animations.end())
	{
		return this->animations.at(name);
	}
	return nullptr;
}

void GrAnimManager::resetAnimation()
{

}

void GrAnimManager::startTransition(float transitionTime)
{
	if (!this->transitioning)
	{
		this->transitioning = true;
		this->transitionTime = transitionTime;
		this->transitionClock = 0.0f;
	}
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

void GrAnimManager::addToActive(string name, float weight, bool loop, void* (*callback)())
{
	GrAnimation* anim = this->getAnimation(name);
	if (anim != nullptr)
	{
		this->addToActive(anim, weight, loop, callback);
	}
	else {
		cout << "Couldn't find " << name << endl;
	}
}

void GrAnimManager::addToActive(GrAnimation* anim, float weight, bool loop, void* (*callback)())
{
	//Don't add an already active animation
	if (!anim->active)
	{
		anim->weight = weight;
		anim->active = true;
		anim->loop = loop;
		this->activeAnimations.addElement(anim);
	}

}

void GrAnimManager::clearAnimCallback(GrAnimation* anim)
{
	anim->resetAnimation();
}

Component* GrAnimManager::clone()
{
	GrAnimManager* cloned = new GrAnimManager();
	map<string, GrAnimation *>::iterator iter;
	for (iter = this->animations.begin(); iter != this->animations.end(); iter++)
	{
		GrAnimation* anim = (iter->second);
		cloned->addAnimation(anim);
	}
	return cloned;
}
