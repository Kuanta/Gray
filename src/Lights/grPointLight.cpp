#include "Lights/grPointLight.h"



grPointLight::grPointLight()
{
	this->color = glm::vec3(1.0f,1.0f,1.0f);
	this->intensity = 1.0f;
	this->constant = 1.0f;
	this->linear = 0.35f;
	this->quadratic = 0.44f;
}

grPointLight::grPointLight(glm::vec3 color, float intensity, float constant, float linear, float quadratic)
{
	this->color = color;
	this->intensity = intensity;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}


grPointLight::~grPointLight()
{
}

void grPointLight::useLight(Shader * shader)
{
	shader->setVec3("pointLight.color", this->color);
	shader->setFloat("pointLight.intensity", this->intensity);
	shader->setVec3("pointLight.position", this->position);
	shader->setFloat("pointLight.linear", this->linear);
	shader->setFloat("pointLight.quadratic", this->quadratic);
	shader->setFloat("pointLight.constant", this->constant);
}

glm::mat4 grPointLight::getTransformMat()
{
	return glm::mat4(1.0f);
}
