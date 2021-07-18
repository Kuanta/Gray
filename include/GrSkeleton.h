#pragma once
#include <map>
#include <vector>
#include <queue>
#include "Components/Component.h"
#include "GrBone.h"
#include "Shader.h"

using namespace std;
class GrSkeleton : public Component
{
public:
	map<string, GrBone*> bones;
	vector<GrBone*> orderedBones; //To send to GPU and store indices
	GrSkeleton();
	~GrSkeleton();
	void addBone(string boneName, GrBone* bone);
	GrBone* getBoneByName(string boneName);
	int getBoneIndex(string boneName);
	glm::mat4 globalInverseTransform;
	vector<glm::mat4> getBoneMatrices();
	void markBone(GrBone* bone);  //Marks the bone for update
	void resetBones(); //Resets bones to their bindPosition

	virtual void start() override;
	virtual void earlyUpdate(float deltaTime) override;
	virtual void lateUpdate(float deltaTime) override;
	virtual void update(float deltaTime) override;
	virtual void draw(Shader* shader) override;
	virtual void cleanup() override;

	std::queue<GrBone*> bonesToUpdate;

	// Inherited via Component
	virtual Component* clone() override;
};

