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
	//this->updateLocalMatrix();
}

void GrBone::setRotation(glm::quat rotation)
{
	this->rotation = rotation;
	//this->updateLocalMatrix();
}

void GrBone::setScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
	//this->updateLocalMatrix();
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
