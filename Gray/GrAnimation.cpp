#include "GrAnimation.h"



GrAnimation::GrAnimation()
{

}

GrAnimation::GrAnimation(string name, double duration, double ticksPerSecond)
{
	this->name = name;
	this->duration = duration;
	this->ticksPerSecond = ticksPerSecond;
}

GrAnimation::~GrAnimation()
{
}

void GrAnimation::update(float deltaTime, GrSkeleton* skeleton)
{
	int currentTick = floor(this->animTime * this->ticksPerSecond);
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
	float factor = (this->animTime - currentTick / this->ticksPerSecond) * this->ticksPerSecond;
	if (skeleton != nullptr)  //Can't animate something without bones
	{
		map<string, GrAnimationNode*>::iterator iter;
		for (iter = this->animNodes.begin(); iter != this->animNodes.end(); iter++)
		{
			GrAnimationNode* animNode = iter->second;
			GrBone* bone = skeleton->getBoneByName(animNode->name);
			if (bone != nullptr)
			{
				if (animNode->keyNum <= 1)  //Some nodes have only 1 frame
				{
					glm::vec3 p1(animNode->posKeys[0].x, animNode->posKeys[0].y, animNode->posKeys[0].z);
					bone->setPosition(p1.x, p1.y, p1.z);
					glm::quat r(animNode->rotKeys[0].w, animNode->rotKeys[0].x, animNode->rotKeys[0].y, animNode->rotKeys[0].z);
					bone->setRotation(r);
					bone->setScale(animNode->scaleKeys[0].x, animNode->scaleKeys[0].y, animNode->scaleKeys[0].z);
				}
				else
				{
					glm::vec3 p1(animNode->posKeys[currentTick].x, animNode->posKeys[currentTick].y, animNode->posKeys[currentTick].z);
					glm::vec3 p2(animNode->posKeys[nextTick].x, animNode->posKeys[nextTick].y, animNode->posKeys[nextTick].z);
					glm::vec3 p = glm::mix(p1, p2, factor);
					bone->setPosition(p.x, p.y, p.z);
					glm::quat r1(animNode->rotKeys[currentTick].w, animNode->rotKeys[currentTick].x, animNode->rotKeys[currentTick].y, animNode->rotKeys[currentTick].z);
					glm::quat r2(animNode->rotKeys[nextTick].w, animNode->rotKeys[nextTick].x, animNode->rotKeys[nextTick].y, animNode->rotKeys[nextTick].z);
					bone->setRotation(glm::slerp(r1, r2, factor));
					bone->setScale(animNode->scaleKeys[currentTick].x, animNode->scaleKeys[currentTick].y, animNode->scaleKeys[currentTick].z);
				}
			}
	
		}
	}
	this->animTime += deltaTime*this->speed; //Animation speed can be adjusted here
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
				//bone->setScale(animNode->scaleKeys[currentTick].x, animNode->scaleKeys[currentTick].y, animNode->scaleKeys[currentTick].z);	
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

void GrAnimationNode::updateBone(GrBone * bone, float animTime, float totalDuration)
{

}

GrAnimFrame GrAnimationNode::getAnimFrame(int i)
{
	GrAnimFrame frame;
	frame.posKey = &this->posKeys.at(i);
	frame.rotKey = &this->rotKeys.at(i);
	frame.scaleKey = &this->scaleKeys.at(i);
	return frame;
}
