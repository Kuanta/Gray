#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <string>
#include <numeric>
#include <iostream>
#include <vector>
#include <assimp/scene.h>
using namespace std;

class GrSkeleton;
class GrBone
{
public:
	GrBone();
	GrBone(GrSkeleton* skeleton, glm::mat4 offsetMatrix,glm::mat4 rootMatrix, string name);
	~GrBone();
	GrSkeleton* skeleton = nullptr;
	GrBone* parentBone = nullptr;
	vector<GrBone*> children;
	string name;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 offsetMatrix;
	glm::mat4 nodeTransformation;
	bool requiresUpdate = false;
	void setPosition(float x, float y, float z);
	void setRotation(glm::quat rotation);
	void setScale(float x, float y, float z);
	void setTargetPosition(float x, float y, float z);
	void setTargetRotation(glm::quat rotation);
	void setTargetScale(float x, float y, float z);
	glm::mat4 getTransformMatrix();
	glm::mat4 getLocalMatrix();
	void updateTransformMatrix();
	void updateLocalMatrix();
	void setTransformMatrix(glm::mat4 transformMatrix);
	void calculateInverseBindMatrix();
	void setToBindPosition();
	void markForUpdate(); //Marks children bones for updates
	void addAnimationFrame(glm::vec3 posKey, glm::quat rotKey, glm::vec3 scaleKey, float weight);
	void blendAnimationFrames();
	GrBone* clone();
	void clearQueues();
	bool transition = false;
	float transitionFactor = 1.0f; //Current p-r-s will be determined by : interpolate(current, target, targetScale)

private:
	bool targetSet = false;
	vector<glm::vec3> posQueue;
	vector<glm::quat> rotQueue;
	vector<glm::vec3> scaleQueue;
	vector<float> animWeights;
	glm::mat4 transformMatrix;
	glm::mat4 localMatrix;
	glm::mat4 bindMatrix;
	glm::mat4 rootMatrix;
	glm::vec3 targetPos;
	glm::vec3 targetScale;
	glm::quat targetRot;
};

