#include "Object.h"
#include "GameManager.h"



Object::Object()
{
	this->position = glm::vec3(0, 0, 0);
	this->rotation = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->gm = nullptr;
}


Object::~Object()
{
}

void Object::setTarget(glm::vec3 newTarget)
{
	this->target = newTarget;
	float radius = sqrt((this->position.x - this->target.x) * (this->position.x - this->target.x) + 
		(this->position.y - this->target.y) * (this->position.y - this->target.y) + 
		(this->position.z - this->target.z) * (this->position.z - this->target.z));
	this->rotation.y = atan2(this->target.z - this->position.z, this->target.x - this->position.z);
	this->rotation.x = asin(radius / (this->target.y - this->position.y));
}

void Object::add(Object * object)
{
	object->setParent(this);
	this->children.addElement(object);
}

void Object::remove(Object * object)
{
	this->children.removeElement(object);
}

void Object::update(float deltaTime)
{
	//Update children
	for (vector<Object*>::iterator it = this->children.elements.begin(); it != this->children.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->update(deltaTime);
		}
	}
	this->children.clearElements();
}

void Object::draw()
{
	//Draw children
	for (vector<Object*>::iterator it = this->children.elements.begin(); it != this->children.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->draw();
		}
	}
}

void Object::cleanup()
{
}
void Object::setPosition(glm::vec3 pos)
{
	this->position = pos;
}

void Object::setPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

void Object::updateModel()
{
	//Get parents
	glm::mat4 model;
	Object* parent = this->parent;
	while (parent != nullptr) {
		model = glm::translate(model, parent->position);
		model = glm::rotate(model, glm::radians(parent->rotation.y), glm::vec3(0, 1, 0)); //Yaw
		model = glm::rotate(model, glm::radians(parent->rotation.x), glm::vec3(1, 0, 0)); //Pitch
		model = glm::rotate(model, glm::radians(parent->rotation.z), glm::vec3(0, 0, 1)); //Roll
		model = glm::scale(model, parent->scale);
		parent = parent->parent;

	}

	model = glm::translate(model, this->position);
	model = glm::rotate(model, glm::radians(this->rotation.y), glm::vec3(0, 1, 0)); //Yaw
	model = glm::rotate(model, glm::radians(this->rotation.x), glm::vec3(1, 0, 0)); //Pitch
	model = glm::rotate(model, glm::radians(this->rotation.z), glm::vec3(0, 0, 1)); //Roll
	model = glm::scale(model, this->scale);

	this->model = model;
}