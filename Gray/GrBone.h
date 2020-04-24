#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <assimp\scene.h>
using namespace std;
class GrBone
{
public:
	GrBone();
	GrBone(glm::mat4 offsetMatrix,glm::mat4 nodeTransformation,string name);
	~GrBone();
	GrBone* parentBone = nullptr;
	string name;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 offsetMatrix;
	glm::mat4 bindMatrix;
	glm::mat4 nodeTransformation;
	void setPosition(float x, float y, float z);
	void setRotation(glm::quat rotation);
	void setScale(float x, float y, float z);
	glm::mat4 getTransformMatrix();
	void updateTransformMatrix();
	void setTransformMatrix(glm::mat4 transformMatrix);
	void calculateInverseBindMatrix();
private:
	glm::mat4 transformMatrix;
};

