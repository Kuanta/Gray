#include "Component.h"

Component::Component()
{
	this->type = ComponentType::DEFAULT;
	this->object = nullptr;
}


Component::~Component()
{
}

void Component::addToObject(Object * object)
{
	this->object = object;
	object->components.push_back(this); //Add this component to the object's component list
	this->start();
}

void Component::removeFromObject()
{
}

void Component::fixedUpdate()
{
}

void Component::draw()
{
}

void Component::draw(Shader* shader)
{
}

void Component::processInput(int key, int scancode, int action, int mods)
{
}
