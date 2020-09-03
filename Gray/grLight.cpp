#include "grLight.h"



GrLight::GrLight()
{
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);
	this->intensity = 1.0f;
}

GrLight::GrLight(glm::vec3 color, float intensity)
{
	this->color = color;
	this->intensity = intensity;
}


GrLight::~GrLight()
{
}

void GrLight::useLight(Shader* shader)
{
	shader->setVec3("ambientLight.color", this->color);
	shader->setFloat("ambientLight.intensity", this->intensity);
}

