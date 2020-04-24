#include "grBone.h"



GrBone::GrBone()
{
}

GrBone::GrBone(glm::mat4 offsetMatrix, glm::mat4 nodeTransformation, string name)
{
	this->scale = glm::vec3(1, 1, 1);
	this->position = glm::vec3(0, 0, 0);
	this->rotation = glm::quat(1, 0, 0, 0);
	this->offsetMatrix = offsetMatrix;
	this->nodeTransformation = nodeTransformation;
	this->name = name;
	this->setTransformMatrix(glm::inverse(this->offsetMatrix));
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

glm::mat4 GrBone::getTransformMatrix()
{
	return this->transformMatrix;
}

void GrBone::updateTransformMatrix()
{
	//Get parents transform
	GrBone* parent = this->parentBone;
	vector<glm::mat4> pMats;
	glm::mat4 parentMatrix;
	while (parent != nullptr)
	{
		glm::mat4 pMat;
		pMat = glm::translate(pMat, parent->position);
		pMat *= glm::mat4_cast(parent->rotation);
		pMat = glm::scale(pMat, parent->scale);
		pMats.insert(pMats.begin(), pMat);
		parent = parent->parentBone;
	}

	for (int i = 0; i < pMats.size(); i++)
	{
		parentMatrix *= pMats.at(i);
	}

	glm::mat4 mat;
	mat = glm::translate(mat, this->position);
	mat *= glm::mat4_cast(this->rotation);
	mat = glm::scale(mat, this->scale);

	this->transformMatrix = parentMatrix * mat;
}

void GrBone::setTransformMatrix(glm::mat4 transformMatrix)
{
	this->transformMatrix = transformMatrix;
}

void GrBone::calculateInverseBindMatrix()
{
	//Loop through parents
	GrBone* parent = this->parentBone;
	vector<glm::mat4> mats;
	while (parent != nullptr) {
		mats.insert(mats.begin(),parent->nodeTransformation);
		parent = parent->parentBone;
	}
	for (int i = 0; i < mats.size(); i++)
	{
		this->bindMatrix *= mats.at(i);
	}
}
