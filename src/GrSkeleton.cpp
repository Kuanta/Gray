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

void GrSkeleton::update(float deltaTime)
{
	GrBone* bone;
	while (!this->bonesToUpdate.empty())
	{
		bone = this->bonesToUpdate.front();
		bone->blendAnimationFrames();
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

void GrSkeleton::draw(Shader* shader)
{
	if (shader != nullptr)
	{
		shader->use();
		vector<glm::mat4> boneMatrices;
		map<string, GrBone*>::iterator it;
		for (it = this->bones.begin(); it != this->bones.end(); it++)
		{
			glm::mat4 boneMatrix = (it->second)->getTransformMatrix();
			boneMatrices.push_back(this->globalInverseTransform * boneMatrix * (it->second)->offsetMatrix);
		}
		if (boneMatrices.size() > 0)
		{
			glUniform1i(glGetUniformLocation(shader->ID, "hasBones"), 1);
			glUniformMatrix4fv(glGetUniformLocation(shader->ID, "gBones"), boneMatrices.size(), GL_FALSE,
				glm::value_ptr(boneMatrices[0]));
		}
		else {
			glUniform1i(glGetUniformLocation(shader->ID, "hasBones"), 0);
		}
	}
}

void GrSkeleton::cleanup()
{
}

Component* GrSkeleton::clone()
{
	GrSkeleton* skeleton = new GrSkeleton();
	skeleton->globalInverseTransform = this->globalInverseTransform;
	map<string, GrBone*>::iterator it;
	for (it = this->bones.begin(); it != this->bones.end(); it++)
	{
		GrBone* cloned = it->second->clone();
		
		if (it->second->parentBone != nullptr) {
			GrBone* parentOfCloned = skeleton->getBoneByName(it->second->parentBone->name);
			if (skeleton->getBoneByName(it->second->parentBone->name) == nullptr)
			{
				parentOfCloned = it->second->parentBone->clone();
			}
			cloned->parentBone = parentOfCloned;
		}
		skeleton->addBone((std::string) it->first, (GrBone*)it->second);
	}
	return skeleton;
}
