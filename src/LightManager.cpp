#include "LightManager.h"

LightManager::LightManager()
{
	this->init();
}

LightManager::~LightManager()
{
}

void LightManager::init()
{

}

void LightManager::add(GrLight* light)
{
	this->lights.addElement(light);
	this->numberOfLights++;
}

void LightManager::remove(GrLight* light)
{
	this->lights.removeElement(light);
	this->numberOfLights--;
}

void LightManager::update(float delta)
{
}

void LightManager::draw()
{
	int counter = 0;
	for (vector<GrLight*>::iterator it = this->lights.elements.begin(); it != this->lights.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			GrLight* light = *it;
			this->lightStructs[counter].type = (int)light->type;
			this->lightStructs[counter].color = glm::vec4(light->color,1);
			this->lightStructs[counter].position = glm::vec4(light->getPosition(),1);
			this->lightStructs[counter].direction = glm::vec4(light->direction,1);
			this->lightStructs[counter].intensity = light->intensity;
			this->lightStructs[counter].constant = light->constant;
			this->lightStructs[counter].linear = light->linear;
			this->lightStructs[counter].quadratic = light->quadratic;
			counter++;
		}
	}
	this->lightsBuffer->bindBuffer();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(this->lightStructs), this->lightStructs);
	glBufferSubData(GL_UNIFORM_BUFFER, 100*80, 4, &this->numberOfLights); //Send the number of lights
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //Unbind
}

void LightManager::clean()
{
}

void LightManager::updateBuffer()
{
	//Number of bytes for a single Light structure in the shader is 
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, this->numberOfLights*11*4, reinterpret_cast<>);
}
