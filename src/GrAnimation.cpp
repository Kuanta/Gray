#include "GrAnimation.h"
#include "GrAnimManager.h"


GrAnimation::GrAnimation()
{

}

GrAnimation::GrAnimation(string name, double duration, double ticksPerSecond)
{
	this->name = name;
	this->ticksPerSecond = ticksPerSecond;
	this->duration = duration/ticksPerSecond;
}

GrAnimation::~GrAnimation()
{
}

void GrAnimation::update(float deltaTime, GrSkeleton* skeleton, bool transition, float transitionFactor)
{
	if (this->animTime > this->duration)
	{
		if (!this->loop)
		{
			// This animation doesn't loop
			this->active = false;
			this->animTime = 0;
			this->animManager->startTransition(0.8f);
			// Call end loop
			return;
		}
		this->animTime -= this->duration;
	}
	if (skeleton != nullptr)  //Can't animate something without bones
	{
		map<string, GrAnimationNode*>::iterator iter;
		for (iter = this->animNodes.begin(); iter != this->animNodes.end(); iter++)
		{
			GrAnimationNode* animNode = iter->second;
			GrBone* bone = skeleton->getBoneByName(animNode->name);
			if (bone != nullptr)
			{
				if(!transition || (transition && !bone->targetSet)){
					//During the transition, animTime doesn't progress so no need to find the same nodes again during transition
					//But to set the target, we check bone->setTarget so that just for a single udpate, we can set the target
					GrAnimFrame f = animNode->getAnimFrame(animTime, duration, this->direction);
					if (animNode->keyNum > 1)
					{
						GrAnimFrame f = animNode->getAnimFrame(animTime, duration, this->direction);
						bone->addAnimationFrame(*f.posKey, *f.rotKey, *f.scaleKey, this->weight);
					}
					else
					{
						bone->addAnimationFrame(animNode->posKeys[0], animNode->rotKeys[0], animNode->scaleKeys[0], this->weight);
					}
					
				}
				if(transition)
				{
					bone->transition = true;
					bone->transitionFactor = transitionFactor;
				}else{
					bone->transition = false;
				}
				bone->updateLocalMatrix();
				bone->markForUpdate();				
			}
		}
	}
	if (!transition)
	{
		this->animTime += deltaTime * this->speed * this->direction; //Animation speed can be adjusted here
	}
}

void GrAnimation::fadeTo(float targetWeight, float speed)
{
	this->fading = true;
	this->targetWeight = targetWeight;
	if (this->targetWeight > this->weight)
	{
		this->fadeSpeed = abs(speed);
	}
	else if (this->targetWeight < this->weight)
	{
		this->fadeSpeed = -abs(speed);
	}
	else {
		this->fading = true;
	}
}

void GrAnimation::transition(float transitionFactor, GrSkeleton* skeleton)
{
	if (skeleton != nullptr)  //Can't animate something without bones
	{
		map<string, GrAnimationNode*>::iterator iter;
		for (iter = this->animNodes.begin(); iter != this->animNodes.end(); iter++)
		{
			GrAnimationNode* animNode = iter->second;
			GrBone* bone = skeleton->getBoneByName(animNode->name);
			if (bone != nullptr)
			{
				bone->transitionFactor = transitionFactor;
				bone->transition = true;
			}else{
				cout<<"Empty "<<animNode->name<<endl;
			}
		}
	}
}

void GrAnimation::resetAnimation()
{
	this->animTime = 0;
}

GrAnimationNode::GrAnimationNode(aiNodeAnim * node)
{
	this->name = node->mNodeName.data;
	this->keyNum = node->mNumPositionKeys;
	for (int i = 0; i < node->mNumPositionKeys; i++)
	{
		aiVector3D aiPosKey = node->mPositionKeys[i].mValue;
		aiQuaternion aiRotKey = node->mRotationKeys[i].mValue;
		aiVector3D aiScaleKey = node->mScalingKeys[i].mValue;
		
		//Glm counter-parts
		glm::vec3 posKey = glm::vec3(aiPosKey.x, aiPosKey.y, aiPosKey.z);
		glm::quat rotKey(aiRotKey.w, aiRotKey.x, aiRotKey.y, aiRotKey.z);
		glm::vec3 scaleKey = glm::vec3(aiScaleKey.x, aiScaleKey.y, aiScaleKey.z);

		this->posKeys.push_back(posKey);
		this->rotKeys.push_back(rotKey);
		this->scaleKeys.push_back(scaleKey);
	}
}


GrAnimFrame GrAnimationNode::getAnimFrame(float relTime, float duration, int direction)
{
	GrAnimFrame frame;
	float currentTick, nextTick;
	float factor;
	float stepTime = duration / ((float)this->keyNum -1.0f);  
	float temp = relTime / stepTime;
	float remainder = relTime - (float)floor(temp) * stepTime;
	factor = remainder / stepTime;
	if (direction == 1) // Forward
	{
		currentTick = (float)floor(temp);
		nextTick = currentTick + 1;
		//factor = (temp - currentTick)/(float)ticksPerSecond;
	}
	else {
		currentTick = (float)ceil(temp);
		nextTick = currentTick - 1;
		//factor = (currentTick - temp)/(float)ticksPerSecond;
	}
	
	if (currentTick >= this->keyNum)
	{
		currentTick = 0;
	}
	else if (currentTick < 0)
	{
		currentTick = this->keyNum-1;
	}

	if (nextTick >= this->keyNum)
	{
		nextTick = 0;
	}
	else if (nextTick < 0)
	{
		nextTick = this->keyNum-1;
	}
	glm::vec3 p = glm::mix(this->posKeys[currentTick], this->posKeys[nextTick], factor);
	glm::quat r = glm::slerp(this->rotKeys[currentTick], this->rotKeys[nextTick], factor);
	glm::vec3 s = glm::mix(this->scaleKeys[currentTick], this->scaleKeys[nextTick], factor);

	frame.posKey = &p;
	frame.rotKey = &r;
	frame.scaleKey = &s;
	return frame;
}
