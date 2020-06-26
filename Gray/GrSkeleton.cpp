#include "GrSkeleton.h"



GrSkeleton::GrSkeleton()
{
	this->type = ComponentType::SKELETON;
}


GrSkeleton::~GrSkeleton()
{
}

void GrSkeleton::addBone(string boneName, GrBone* bone)
{
	this->bones.insert(std::pair<string, GrBone*>(boneName, bone));
}

GrBone * GrSkeleton::getBoneByName(string boneName)
{
	try {
		return this->bones.at(boneName);
	}
	catch(exception e){ // Bone with that name doesn't exist probably
		return nullptr;
	}
}

void GrSkeleton::markBone(GrBone* bone)
{
	this->bonesToUpdate.push(bone);
}

void GrSkeleton::start()
{
}

void GrSkeleton::update(float deltaTime)
{
	GrBone* bone;
	while (!this->bonesToUpdate.empty())
	{
		bone = this->bonesToUpdate.front();
		bone->updateTransformMatrix();
		bone->requiresUpdate = false;
		this->bonesToUpdate.pop();
	}
	/*map<string, GrBone*>::iterator iter;
	for (iter = this->bones.begin(); iter != this->bones.end(); iter++)
	{
		GrBone* bone = iter->second;
		bone->updateTransformMatrix();
	}*/
}

void GrSkeleton::cleanup()
{
}
