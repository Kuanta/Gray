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
	if(this->bones.find(boneName) == this->bones.end())
	{
		//No such key
		this->bones.insert(std::pair<string, GrBone *>(boneName, bone));
		this->orderedBones.push_back(bone);
	}

}
int GrSkeleton::getBoneIndex(string boneName)
{
	for(int i=0;i<this->orderedBones.size();i++)
	{
		if (!boneName.compare(this->orderedBones.at(i)->name))
		{
			return i;
		}
	}
	return -1; //Couldn't find the bone
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

vector<glm::mat4> GrSkeleton::getBoneMatrices()
{
	vector<glm::mat4> boneMatrices;
	map<string, GrBone*>::iterator it;
	for (it = this->bones.begin(); it != this->bones.end(); it++)
	{
		glm::mat4 boneMatrix = (it->second)->getTransformMatrix();
		boneMatrices.push_back(this->globalInverseTransform * boneMatrix * (it->second)->offsetMatrix);
	}
	return boneMatrices;
}

void GrSkeleton::markBone(GrBone* bone)
{
	this->bonesToUpdate.push(bone);
}

void GrSkeleton::start()
{
}

void GrSkeleton::earlyUpdate(float deltaTime)
{
	GrBone *bone;
	while (!this->bonesToUpdate.empty())
	{
		bone = this->bonesToUpdate.front();
		bone->blendAnimationFrames();
		bone->updateTransformMatrix();
		bone->requiresUpdate = false;
		this->bonesToUpdate.pop();
	}
}
void GrSkeleton::lateUpdate(float deltaTime)
{

}
void GrSkeleton::update(float deltaTime)
{

}

void GrSkeleton::draw(Shader* shader)
{
	if (shader != nullptr)
	{
		shader->use();
		vector<glm::mat4> boneMatrices;
		vector<GrBone*>::iterator it;
		for (int i=0;i<orderedBones.size();i++)
		{
			GrBone* bone = orderedBones.at(i);
			glm::mat4 boneMatrix = bone->getTransformMatrix();
			boneMatrices.push_back(this->globalInverseTransform * boneMatrix * bone->offsetMatrix);
		}
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "gBones"), boneMatrices.size(), GL_FALSE,
		glm::value_ptr(boneMatrices[0]));
	}
}

void GrSkeleton::cleanup()
{
}

Component* GrSkeleton::clone()
{
	GrSkeleton* newSkeleton = new GrSkeleton();
	newSkeleton->globalInverseTransform = this->globalInverseTransform;

	for(int i=0;i<this->orderedBones.size();i++)
	{
		GrBone* bone = this->orderedBones.at(i);
		GrBone* cloned = bone->clone();
		cloned->requiresUpdate = false;
		cloned->skeleton = newSkeleton;
		newSkeleton->addBone(cloned->name, cloned);
	}
	//Parent relations
	map<string, GrBone*>::iterator it;
	for (it = this->bones.begin(); it != this->bones.end(); it++)
	{
		GrBone* bone = it->second;
		if (it->second->parentBone != nullptr) {
			GrBone *clonedBone = newSkeleton->getBoneByName(bone->name);
			GrBone *parentOfCloned = newSkeleton->getBoneByName(it->second->parentBone->name);
			clonedBone->parentBone = parentOfCloned;
			if(parentOfCloned != nullptr)
			{
				parentOfCloned->children.push_back(clonedBone);
			}
		}
	}
	for(int i=0;i<newSkeleton->orderedBones.size();i++)
	{
		GrBone *bone = newSkeleton->orderedBones.at(i);
		bone->calculateInverseBindMatrix();
		bone->setToBindPosition();
	}
	return newSkeleton;
}
