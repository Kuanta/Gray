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

void GrSkeleton::start()
{
}

void GrSkeleton::update(float deltaTime)
{
}

void GrSkeleton::cleanup()
{
}
