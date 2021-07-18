#include "GrBone.h"
#include "GrSkeleton.h"


GrBone::GrBone()
{
}

GrBone::GrBone(GrSkeleton* skeleton, glm::mat4 offsetMatrix, glm::mat4 rootMatrix, string name)
{
	this->skeleton = skeleton;
	this->scale = glm::vec3(1, 1, 1);
	this->position = glm::vec3(0, 0, 0);
	this->rotation = glm::quat(1, 0, 0, 0);
	this->offsetMatrix = offsetMatrix;
	this->rootMatrix = rootMatrix;
	this->name = name;
	this->updateLocalMatrix();
	//this->localMatrix = nodeTransformation;
	//glm::decompose(this->localMatrix, this->scale, this->rotation, this->position, glm::vec3(), glm::vec4());
	//this->setTransformMatrix(nodeTransformation);
	//this->transformMatrix = glm::inverse(this->offsetMatrix);
}


GrBone::~GrBone()
{
}

void GrBone::setPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void GrBone::setRotation(glm::quat rotation)
{
	this->rotation = rotation;
}

void GrBone::setScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
}

void GrBone::setTargetPosition(float x, float y, float z)
{
	this->targetPos.x = x;
	this->targetPos.y = y;
	this->targetPos.z = z;
}

void GrBone::setTargetRotation(glm::quat rotation)
{
	this->targetRot = rotation;
}

void GrBone::setTargetScale(float x, float y, float z)
{
	this->targetScale.x = x;
	this->targetScale.y = y;
	this->targetScale.z = z;
}

glm::mat4 GrBone::getTransformMatrix()
{
	return this->transformMatrix;
}

glm::mat4 GrBone::getLocalMatrix()
{
	return this->localMatrix;
}

void GrBone::updateTransformMatrix()
{
	//Get parents transform
	GrBone* parent = this->parentBone;
	if(parent == nullptr)
	{
		this->transformMatrix = this->localMatrix;
		return;
	}
	vector<glm::mat4> pMats;
	glm::mat4 parentMatrix= glm::mat4(1.0f);
	while (parent != nullptr)
	{
		pMats.insert(pMats.begin(), parent->getLocalMatrix());
		parent = parent->parentBone;
	}

	for (int i = 0; i < pMats.size(); i++)
	{
		parentMatrix *= pMats.at(i);
	}

	this->transformMatrix = parentMatrix * this->localMatrix;
	
}

void GrBone::updateLocalMatrix()
{
	glm::mat4 mat= glm::mat4(1.0f);
	if (this->transition && this->targetSet)
	{
		this->position = glm::mix(this->position, this->targetPos, this->transitionFactor);
		this->rotation = glm::slerp(this->rotation, this->targetRot, this->transitionFactor);
		this->scale = glm::mix(this->scale, this->targetScale, this->transitionFactor);
	}

	mat = glm::translate(mat, this->position);
	mat *= glm::mat4_cast(this->rotation);
	mat = glm::scale(mat, this->scale);
	this->localMatrix = mat;
}

void GrBone::setTransformMatrix(glm::mat4 transformMatrix)
{
	this->transformMatrix = transformMatrix;
	glm::vec3 unitVec3;
	glm::vec4 unitVec4;
	glm::decompose(transformMatrix, this->scale, this->rotation, this->position, unitVec3, unitVec4);

	//this->rotation = glm::eulerAngles(orientation);
}

void GrBone::calculateInverseBindMatrix()
{
	//Loop through parents
	GrBone* parent = this->parentBone;
	//vector<glm::mat4> mats;
	//while (parent != nullptr) {
	//	mats.insert(mats.end(),glm::inverse(parent->offsetMatrix));
	//	parent = parent->parentBone;
	//}
	//this->bindMatrix = this->offsetMatrix;
	//for (int i = 0; i < mats.size(); i++)
	//{
	//	this->bindMatrix = this->bindMatrix* mats.at(i);
	//}
	if (this->parentBone != nullptr)
	{
		this->bindMatrix = this->parentBone->offsetMatrix * glm::inverse(this->offsetMatrix);
	}
	else
	{
		this->bindMatrix = glm::inverse(this->offsetMatrix);
	}
}

void GrBone::setToBindPosition()
{
	glm::vec3 unitVec3;
	glm::vec4 unitVec4;
	glm::decompose(this->bindMatrix, this->scale, this->rotation, this->position, unitVec3, unitVec4);
	this->updateLocalMatrix();
	this->markForUpdate();
}

void GrBone::markForUpdate()
{
	if (!this->requiresUpdate)
	{
		this->skeleton->markBone(this);
		this->requiresUpdate = true;

		//Update the children too
		for (vector<GrBone*>::iterator it = this->children.begin(); it != this->children.end(); it++)
		{
			//It may have been already marked
			if (!(*it)->requiresUpdate)
			{
				//this->skeleton->markBone((*it));
				(*it)->markForUpdate();
			}
		}
	}
}

void GrBone::addAnimationFrame(glm::vec3 posKey, glm::quat rotKey, glm::vec3 scaleKey, float weight)
{
	this->posQueue.push_back(posKey);
	this->rotQueue.push_back(rotKey);
	this->scaleQueue.push_back(scaleKey);
	this->animWeights.push_back(weight);
}

void GrBone::blendAnimationFrames()
{
	glm::vec3 _targetPos(0.0f, 0.0f, 0.0f);
	glm::vec3 _targetScale(0.0f, 0.0f, 0.0f);
	glm::vec3 _targetRot(0.0f, 0.0f, 0.0f);
	float totalWeights = 0.0f;
	for (int i = 0; i < this->posQueue.size(); i++)
	{
		float weight = this->animWeights.at(i);
		totalWeights += weight;
		_targetPos += this->posQueue.at(i) * weight;
		_targetScale += this->scaleQueue.at(i) * weight;
		_targetRot += glm::eulerAngles(this->rotQueue.at(i)) * weight;

	}
	if (totalWeights > 0)
	{
		_targetPos = _targetPos / totalWeights;
		_targetRot = _targetRot / totalWeights;
		_targetScale = _targetScale / totalWeights;

		if (this->transition)
		{
			this->setTargetPosition(_targetPos.x, _targetPos.y, _targetPos.z);
			this->setTargetRotation(glm::quat(_targetRot));
			this->setTargetScale(_targetScale.x, _targetScale.y, _targetScale.z);
			this->targetSet = true;
		}
		else
		{
			this->setPosition(_targetPos.x, _targetPos.y, _targetPos.z);
			this->setRotation(glm::quat(_targetRot));
			this->setScale(_targetScale.x, _targetScale.y, _targetScale.z);
		}
		//this->updateLocalMatrix();
	}
	
	this->clearQueues();  //There can still be frames in queues even if the weights are zero. For example an animation with zero weight affecting a bone. If this bone
	//is not affected by the remaining animations, the queue for that bone my grow without end. 
}

GrBone* GrBone::clone()
{
	GrBone* bone = new GrBone();
	bone->scale = glm::vec3(1, 1, 1);
	bone->position = glm::vec3(0, 0, 0);
	bone->rotation = glm::quat(1, 0, 0, 0);
	bone->offsetMatrix = this->offsetMatrix;
	bone->rootMatrix = this->rootMatrix;
	bone->name = this->name;
	bone->bindMatrix = this->bindMatrix;
	bone->requiresUpdate = false; 
	bone->updateLocalMatrix();
	return bone;
}

void GrBone::clearQueues()
{
	this->posQueue.clear();
	this->rotQueue.clear();
	this->scaleQueue.clear();
	this->animWeights.clear();
}
