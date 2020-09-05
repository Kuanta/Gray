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
	glGenBuffers(1, &this->lightsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, this->lightsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, LIGHT_BUFFER_SIZE, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightManager::add(GrLight* light)
{
	this->lights.addElement(light);
}

void LightManager::remove(GrLight* light)
{
	this->lights.removeElement(light);
}

void LightManager::update(float delta)
{
}

void LightManager::draw()
{
}

void LightManager::clean()
{
}

void LightManager::updateBuffer()
{

}
