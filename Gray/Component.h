#pragma once

#include "Object.h"
#include "Shader.h"

enum ComponentType {
	DEFAULT,
	MESH,
	ANIMATION_MANAGER,
	SKELETON

};

class Component
{
public:
	ComponentType type;
	Component();
	~Component();
	void addToObject(Object* object);
	void removeFromObject();
	virtual void start() = 0; //Will be called when the component is added to an object
	virtual void update(float deltaTime) = 0;
	virtual void fixedUpdate();
	virtual void cleanup() = 0;
	virtual void draw();
	virtual void processInput(int key, int scancode, int action, int mods);
	Object* object; //Object that this component belongs to
};