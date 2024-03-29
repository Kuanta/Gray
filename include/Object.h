#pragma once

#include <math.h>
#include <typeinfo>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ElementManager.h"
#include "Shader.h"

class GameManager;
class Component;
enum ComponentType : int;

class Object
{
public:
	Object();
	~Object();
	Object* parent;
	GameManager* gm;
	ElementManager<Object*> children;
	vector<Component*> components;
	string name;
	int id;
	bool isStatic = true; //For shadow calculations
	
	glm::vec3 target;
	glm::mat4 model;
	//Methods
	void setShader(Shader* shader);
	Shader* getShader();
	void setPosition(glm::vec3 pos);
	void setRotation(glm::vec3 rot);
	void setScale(glm::vec3 scale);
	void displace(glm::vec3 pos);
	void updateScale(glm::vec3 scale);
	void setPosition(float x, float y, float z);
	void setPositionX(float x) { this->position.x = x; updateLocalMatrix();}
	void setPositionY(float y) { this->position.y = y; updateLocalMatrix();}
	void setPositionZ(float z) { this->position.z = z; updateLocalMatrix();}
	void setRotation(float x, float y, float z);
	void setRotationX(float x) { this->rotation.x = x; updateLocalMatrix();}
	void setRotationY(float y) { this->rotation.y = y; updateLocalMatrix();}
	void setRotationZ(float z) { this->rotation.z = z; updateLocalMatrix();}
	void setScale(float x, float y, float z);
	void setScaleX(float x) { this->scale.x = x; updateLocalMatrix();}
	void setScaleY(float y) { this->scale.y = y; updateLocalMatrix();}
	void setScaleZ(float z) { this->scale.z = z; updateLocalMatrix();}
	void displace(float x, float y, float z);
	void updateScale(float x, float y, float z);

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	glm::vec3 getForward();
	glm::vec3 getRight();

	Object* getRootObject(); //Returns the greatest grandparent
	Object* clone(Object* parent=nullptr);
	void addComponent(Component* component);
	void setTarget(glm::vec3 newTarget); // Updates rotations from target
	void add(Object* object);
	void remove(Object* object);
	virtual void earlyUpdate(float deltaTime);
	virtual void lateUpdate(float deltaTime);
	virtual void update(float deltaTime);
	virtual void draw(Shader* shader);
	virtual void draw();
	void cleanup();

	Component* getComponentByType(ComponentType type);
	Object* getChildWithByName(string name);
	void loopThroughChildren(Component* comp, void (*callback)(const void*, const void*));
	void updateLocalMatrix();
	glm::mat4 getLocalMatrix();
	void setLocalMatrix(glm::mat4 localMatrix);
	void setParent(Object *parent)
	{
		this->parent = parent;
		this->gm = parent->gm;
	}
	void updateModel();
	void updateChildModels(glm::mat4 parentModel); //A recursice function to update child nodes
	bool requireModelUpdate = false;
	Shader *shader = nullptr;
	bool castShadow = true;
	bool receiveShadow = true;

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 localMatrix;

	
};

