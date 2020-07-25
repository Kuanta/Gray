#include "grBone.h"
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
	//this->updateLocalMatrix();
	GrBone* parent = this->parentBone;
	vector<glm::mat4> pMats;
	glm::mat4 parentMatrix;
	while (parent != nullptr)
	{
		//glm::mat4 pMat;
		//pMat = glm::translate(pMat, parent->position);
		//pMat *= glm::mat4_cast(parent->rotation);
		//pMat = glm::scale(pMat, parent->scale);
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
	glm::mat4 mat;
	mat = glm::translate(mat, this->position);
	mat *= glm::mat4_cast(this->rotation);
	mat = glm::scale(mat, this->scale);
	this->localMatrix = mat;
}

void GrBone::setTransformMatrix(glm::mat4 transformMatrix)
{
	this->transformMatrix = transformMatrix;
	glm::decompose(transformMatrix, this->scale, this->rotation, this->position, glm::vec3(), glm::vec4());

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
	glm::decompose(this->bindMatrix, this->scale, this->rotation, this->position, glm::vec3(), glm::vec4());
	this->updateLocalMatrix();
	this->markForUpdate();
}

void GrBone::markForUpdate()
{
	if (!this->requiresUpdate)
	{
		this->skeleton->markBone(this);
		this->requiresUpdate = true;
	}
	
	//Update the children too
	for (vector<GrBone*>::iterator it = this->children.begin(); it != this->children.end(); it++)
	{
		//It may have been already marked
		if (!(*it)->requiresUpdate)
		{
			this->skeleton->markBone((*it));
			(*it)->markForUpdate();
			(*it)->requiresUpdate = true;
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
	if (this->posQueue.size() == 1)
	{
		float weight = this->animWeights.at(0);
		if (weight > 0)
		{
			glm::vec3 posKey = this->posQueue.at(0);
			glm::quat rotKey = this->rotQueue.at(0);
			glm::vec3 scaleKey = this->scaleQueue.at(0);
			this->setPosition(posKey.x, posKey.y, posKey.z);
			this->setRotation(rotKey);
			this->setScale(scaleKey.x, scaleKey.y, scaleKey.z);
		}

	}
	else if(this->posQueue.size() > 1){
		glm::vec3 currPos = this->posQueue.at(0);
		glm::quat currRot = this->rotQueue.at(0);
		glm::vec3 currScale = this->scaleQueue.at(0);
		for (int i = 1; i < this->posQueue.size(); i++)
		{
			float preWeight = this->animWeights.at(i-1);
			glm::vec3 posKey = this->posQueue.at(i);
			glm::quat rotKey = this->rotQueue.at(i);
			glm::vec3 scaleKey = this->scaleQueue.at(i);
			float weight = this->animWeights.at(i);
			float relWeight;
			if (preWeight + weight == 0)
			{
				relWeight = 0;
			}
			else
			{
				relWeight = weight / (preWeight + weight);
			}
			currPos = glm::mix(currPos, posKey, relWeight);
			currRot = glm::slerp(currRot, rotKey,  relWeight);
			currScale = glm::mix(currScale, scaleKey, relWeight);
		}
		this->position = currPos;
		this->setRotation(currRot);
		this->scale = currScale;
	}
	this->posQueue.clear();
	this->rotQueue.clear();
	this->scaleQueue.clear();
	this->animWeights.clear();
	this->updateLocalMatrix();
}
