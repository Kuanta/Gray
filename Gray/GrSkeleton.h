#pragma once
#include <map>
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

	// Component arac�l���yla devral�nd�
	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void cleanup() override;
};

