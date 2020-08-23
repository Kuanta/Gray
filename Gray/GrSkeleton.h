#pragma once
#include <map>
#include <queue>
#include "Component.h"
#include "GrBone.h"
#include "Shader.h"

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
	vector<glm::mat4> getBoneMatrices();
	void markBone(GrBone* bone);  //Marks the bone for update
	void resetBones(); //Resets bones to their bindPosition

	// Component aracýlýðýyla devralýndý
	virtual void start() override;
	virtual void update(float deltaTime) override;
	virtual void draw(Shader* shader) override;
	virtual void cleanup() override;
private:
	std::queue<GrBone*> bonesToUpdate;

	// Inherited via Component
	virtual Component* clone() override;
};

