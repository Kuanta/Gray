#include "grDirectionalLight.h"



grDirectionalLight::grDirectionalLight():GrLight()
{
	this->direction = glm::vec3(0,-1,0);
}

grDirectionalLight::grDirectionalLight(glm::vec3 color, float intensity, glm::vec3 direction):GrLight(color, intensity)
{
	this->direction = direction;
}


grDirectionalLight::~grDirectionalLight()
{
}

void grDirectionalLight::useLight(Shader * shader)
{
	shader->setVec3("directionalLight.color", this->color);
	shader->setVec3("directionalLight.direction", this->direction);
	shader->setFloat("directionalLight.intensity", this->intensity);
}
