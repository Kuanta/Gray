#include "GrPbrLight.h"

void GrPbrLight::useLight(Shader* shader)
{
	shader->setVec3("pointLight.color", this->color);
	shader->setFloat("pointLight.intensity", this->intensity);
	shader->setVec3("pointLight.position", this->position);
}
