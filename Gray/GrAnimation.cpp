#include "GrAnimation.h"



GrAnimation::GrAnimation()
{

}

GrAnimation::GrAnimation(string name, double duration, double ticksPerSecond)
{
	this->name = name;
	this->duration = duration/ticksPerSecond;
	this->ticksPerSecond = ticksPerSecond;
}

GrAnimation::~GrAnimation()
{
}

void GrAnimation::update(float deltaTime, GrSkeleton* skeleton)
{
	if (this->animTime > this->duration)
	{
		this->animTime -= this->duration;
	}
	
	/*int currentTick = floor(this->animTime * this->ticksPerSecond);
	int nextTick = currentTick + this->direction;

	if (currentTick > this->duration)
	{
		currentTick = 0;
		this->animTime = 0;
	}
	if (nextTick > this->duration)
	{
		nextTick = 0;
	}
	float factor = (this->animTime - currentTick / this->ticksPerSecond) * this->ticksPerSecond;*/
	if (skeleton != nullptr)  //Can't animate something without bones
	{
		map<string, GrAnimationNode*>::iterator iter;
		for (iter = this->animNodes.begin(); iter != this->animNodes.end(); iter++)
		{
			GrAnimationNode* animNode = iter->second;
			GrBone* bone = skeleton->getBoneByName(animNode->name);
			if (bone != nullptr)
			{
				if (animNode->keyNum > 1)
				{
					GrAnimFrame f = animNode->getAnimFrame(animTime / duration, this->ticksPerSecond, this->direction);
					bone->setPosition(f.posKey->x, f.posKey->y, f.posKey->z);
					bone->setRotation(*f.rotKey);
					bone->setScale(f.scaleKey->x, f.scaleKey->y, f.scaleKey->z);
				}
				else
				{
					bone->setPosition(animNode->posKeys[0].x, animNode->posKeys[0].y, animNode->posKeys[0].z);
					bone->setRotation(animNode->rotKeys[0]);
					bone->setScale(animNode->scaleKeys[0].x, animNode->scaleKeys[0].y, animNode->scaleKeys[0].z);
				}
				bone->updateLocalMatrix();
				bone->markForUpdate();
			}
	
		}
	}
	this->animTime += deltaTime*this->speed*this->direction; //Animation speed can be adjusted here
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
				glm::vec3 p1(bone->position.x, bone->position.y, bone->position.z);
				glm::vec3 p2(animNode->posKeys[0].x, animNode->posKeys[0].y, animNode->posKeys[0].z);
				glm::vec3 p = glm::mix(p1, p2, transitionFactor);
				bone->setPosition(p.x, p.y, p.z);
				glm::quat r1 = glm::quat(bone->rotation);
				glm::quat r2(animNode->rotKeys[0].w, animNode->rotKeys[0].x, animNode->rotKeys[0].y, animNode->rotKeys[0].z);
				bone->setRotation(glm::slerp(r1, r2, transitionFactor));
				bone->updateLocalMatrix();
				bone->markForUpdate();
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


GrAnimFrame GrAnimationNode::getAnimFrame(float relTime, float ticksPerSecond, int direction)
{
	GrAnimFrame frame;
	int currentTick, nextTick;
	float factor;
	float temp = this->keyNum * relTime;
	if (direction == 1) // Forward
	{
		currentTick = floor(temp);
		nextTick = currentTick + 1;
		factor = (temp - currentTick)/ticksPerSecond;
	}
	else {
		currentTick = ceil(temp);
		nextTick = currentTick - 1;
		factor = (currentTick - temp)/ticksPerSecond;
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
	//bone->setPosition(p.x, p.y, p.z);
	glm::quat r = glm::slerp(this->rotKeys[currentTick], this->rotKeys[nextTick], factor);
	glm::vec3 s = glm::mix(this->scaleKeys[currentTick], this->scaleKeys[nextTick], factor);

	frame.posKey = &p;
	frame.rotKey = &r;
	frame.scaleKey = &s;
	return frame;
}
