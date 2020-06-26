#include "Object.h"
#include "GameManager.h"
#include "Component.h"


Object::Object()
{
	this->position = glm::vec3(0, 0, 0);
	this->rotation = glm::vec3(0, 0, 0);
	this->scale = glm::vec3(1, 1, 1);
	this->updateLocalMatrix();
	this->gm = nullptr;
}


Object::~Object()
{
}

Object * Object::getRootObject()
{
	Object* rootObject = this;
	while (rootObject->parent != nullptr)
	{
		rootObject = rootObject->parent;
	}
	return rootObject;
}

void Object::addComponent(Component * component)
{
	bool componentExists = false;
	for (vector<Component*>::iterator iter = this->components.begin(); iter != this->components.end(); iter++)
	{
		if ((*iter) == component)
		{
			componentExists = true;
			break;
		}
	}

	if (!componentExists)
	{
		component->addToObject(this);
	}
}

void Object::setTarget(glm::vec3 newTarget)
{
	this->target = newTarget;
	float radius = sqrt((this->position.x - this->target.x) * (this->position.x - this->target.x) + 
		(this->position.y - this->target.y) * (this->position.y - this->target.y) + 
		(this->position.z - this->target.z) * (this->position.z - this->target.z));
	this->rotation.y = atan2(this->target.z - this->position.z, this->target.x - this->position.z);
	this->rotation.x = asin(radius / (this->target.y - this->position.y));
	this->updateLocalMatrix();
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
	//Update components
	for (vector<Component*>::iterator iter = this->components.begin(); iter != this->components.end(); iter++)
	{
		(*iter)->update(deltaTime);
	}
	//Update children
	for (vector<Object*>::iterator it = this->children.elements.begin(); it != this->children.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->update(deltaTime);
		}
	}
	this->children.clearElements();

	if (this->requireModelUpdate)
	{
		this->updateModel();
		this->requireModelUpdate = false;
	}
}

void Object::draw()
{
	//Draw components
	for (vector<Component*>::iterator iter = this->components.begin(); iter != this->components.end(); iter++)
	{
		if ((*iter) != nullptr)
		{
			(*iter)->draw();
		}
	}

	//Draw Child objects
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
	for (vector<Component*>::iterator iter = this->components.begin(); iter != this->components.end(); iter++)
	{
		(*iter)->cleanup();
		delete (*iter);
	}
}
void Object::setPosition(glm::vec3 pos)
{
	this->position = pos;
	this->updateLocalMatrix();
}

void Object::setRotation(glm::vec3 rot)
{
	this->rotation = rot;
	this->updateLocalMatrix();
}

void Object::setScale(glm::vec3 scale)
{
	this->scale = scale;
	this->updateLocalMatrix();
}

void Object::displace(glm::vec3 pos)
{
	this->position += pos;
	this->updateLocalMatrix();
}

void Object::updateScale(glm::vec3 scale)
{
	this->scale = this->scale*scale;
	this->updateLocalMatrix();
}

void Object::setPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->updateLocalMatrix();
}

void Object::setRotation(float x, float y, float z)
{
	this->rotation.x = x;
	this->rotation.y = y;
	this->rotation.z = z;
	this->updateLocalMatrix();
}

void Object::setScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;
	this->updateLocalMatrix();
}

void Object::displace(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	this->updateLocalMatrix();
}

void Object::updateScale(float x, float y, float z)
{
	this->scale.x *= x;
	this->scale.y *= y;
	this->scale.z *= z;
	this->updateLocalMatrix();
}

glm::vec3 Object::getPosition()
{
	return this->position;
}

glm::vec3 Object::getRotation()
{
	return this->rotation;
}

glm::vec3 Object::getScale()
{
	return this->scale;
}

Component* Object::getComponentByType(ComponentType type)
{
	for (int i = 0; i < this->components.size(); i++)
	{
		if (this->components[i]->type == type)
		{
			return this->components[i];
		}
	}
	return nullptr;  //If cant find return null
}

Object * Object::getChildWithByName(string name)
{
	if (this->children.elements.size() > 0)
	{
		Object* result = nullptr;
		for (vector<Object*>::iterator it = this->children.elements.begin(); it != this->children.elements.end(); it++)
		{
			if ((*it)->name == name)
			{
				return (*it);
			}
			else {
				result = (*it)->getChildWithByName(name);
				if (result != nullptr)
				{
					return result;
				}
			}
		}
		return result;
	}
	else
	{
		return nullptr;
	}
	
	
}

void Object::loopThroughChildren(Component* comp, void(*callback)(const void*, const void*))
{
	for (vector<Object*>::iterator it = this->children.elements.begin(); it != this->children.elements.end(); it++)
	{
		callback((*it), comp);
		(*it)->loopThroughChildren(comp, callback);
	}
}

void Object::updateLocalMatrix()
{
	glm::mat4 model;
	model = glm::translate(model, this->position);
	model = glm::rotate(model, glm::radians(this->rotation.y), glm::vec3(0, 1, 0)); //Yaw
	model = glm::rotate(model, glm::radians(this->rotation.x), glm::vec3(1, 0, 0)); //Pitch
	model = glm::rotate(model, glm::radians(this->rotation.z), glm::vec3(0, 0, 1)); //Roll
	model = glm::scale(model, this->scale);
	this->localMatrix = model;
	this->requireModelUpdate = true;
}

glm::mat4 Object::getLocalMatrix()
{
	return this->localMatrix;
}

void Object::updateModel()
{
	//Get parents
	glm::mat4 model;
	Object* parent = this->parent;
	while (parent != nullptr) {
		model *= parent->getLocalMatrix();
		parent = parent->parent;

	}

	model *= this->localMatrix;
	this->model = model;
}