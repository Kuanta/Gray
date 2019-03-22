#include "grLight.h"



grLight::grLight()
{
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);
	this->intensity = 1.0f;
}

grLight::grLight(glm::vec3 color, float intensity)
{
	this->color = color;
	this->intensity = intensity;
}


grLight::~grLight()
{
}

void grLight::useLight(Shader* shader)
{
	shader->setVec3("ambientLight.color", this->color);
	shader->setFloat("ambientLight.intensity", this->intensity);
}

