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
		factor = deltaTime / this->transitionTime;
	}
	map<string, GrAnimation>::iterator iter;
	linked::Node<GrAnimation*>* currNode = this->activeAnimatons.head;
	while (currNode != nullptr)
	{
		linked::Node<GrAnimation*>* nextNode = currNode->next;
		GrAnimation* anim = (GrAnimation*)currNode->data;
		if (!anim->active)
		{
			//This animation has been deactivated, remove it
			this->activeAnimatons.popElement(currNode);
		}
		else {
			anim->update(deltaTime, skeleton, this->transitioning, factor);
		}
		currNode = nextNode;
	}
	if (this->transitioning)
	{
		this->transitionTime -= deltaTime;
		if (deltaTime > this->transitionTime)
		{
			this->transitioning = false;
		}
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
		this->activeAnimatons.addElement(anim);
	}

}

void GrAnimManager::clearAnimCallback(GrAnimation* anim)
{
	anim->resetAnimation();
}

Component* GrAnimManager::clone()
{
	GrAnimManager* cloned = new GrAnimManager();
	return cloned;
}
