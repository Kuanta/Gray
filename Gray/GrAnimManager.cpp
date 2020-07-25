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
				anim->update(deltaTime, skeleton);
			}
			currNode = nextNode;
		}

	/*	if (this->currentAnimation)
		{
			
			this->currentAnimation->update(deltaTime, skeleton);
		}*/
	}
	else
	{
		float factor;
		if (deltaTime > this->transitionTime)
		{
			factor = 1.0f;
			this->transitioning = false;
			this->addToActive(this->targetAnimation,1.0f);
			//this->currentAnimation = this->targetAnimation;
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

void GrAnimManager::changeAnimation(string animName, float transitionTime, bool loop)
{
	if (this->animations.find(animName) != this->animations.end())
	{
		this->changeAnimation(this->animations.at(animName),transitionTime);
	}
	else {
		cout << "Couldn't find " << animName << endl;
	}
}

void GrAnimManager::changeAnimation(int animId, float transitionTime, bool loop)
{
}

void GrAnimManager::changeAnimation(GrAnimation* newAnim, float transitionTime, bool loop)
{
	//this->activeAnimations.clearElements();
	newAnim->loop = true;
	this->transitionTo(newAnim, transitionTime);
	
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

void GrAnimManager::addToActive(string name, float weight, bool loop)
{
	GrAnimation* anim = this->getAnimation(name);
	if (anim != nullptr)
	{
		this->addToActive(anim, weight, loop);
	}
	else {
		cout << "Couldn't find " << name << endl;
	}
}

void GrAnimManager::addToActive(GrAnimation* anim, float weight, bool loop)
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
