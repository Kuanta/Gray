#pragma once

#include <math.h>
#include <glm\glm.hpp>
#include "ElementManager.h"

class GameManager;
class Object
{
public:
	Object();
	~Object();
	Object* parent;
	GameManager* gm;
	ElementManager<Object*> children;
	int id;
	//Transformation Variables
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 target;
	glm::mat4 model;
	//Methods
	void setTarget(glm::vec3 newTarget); // Updates rotations from target
	void add(Object* object);
	void remove(Object* object);
	virtual void update(float deltaTime);
	virtual void draw();
	virtual void cleanup();
	void setPosition(glm::vec3 pos);
	void setPosition(float x, float y, float z);
protected:
	void setParent(Object* parent)
	{
		this->parent = parent;
	}
	void updateModel();
};

