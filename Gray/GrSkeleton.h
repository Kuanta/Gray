#pragma once
#include <map>
#include <queue>
#include "Component.h"
#include "GrBone.h"

using namespace std;
class GrSkeleton : public Component
{
public:
	map<string, GrBone*> bones;
	GrSkeleton();
	~GrSkeleton();
	void addBone(string boneName, GrBone* bone);
	GrBone* getBoneByName(string boneName);
	glm::mat4 globalInverseTransform;
	void markBone(GrBone* bone);  //Marks the bone for update
	void resetBones(); //Resets bones to their bindPosition

	// Component aracýlýðýyla devralýndý
	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void cleanup() override;
private:
	std::queue<GrBone*> bonesToUpdate;
};

