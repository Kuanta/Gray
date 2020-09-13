#include "grLight.h"



GrLight::GrLight()
{
	this->type = LIGHT_TYPES::AMBIENT;
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);
	this->intensity = 1.0f;
}

GrLight::GrLight(LIGHT_TYPES type)
{
	this->type = type;
}

GrLight::GrLight(LIGHT_TYPES type, glm::vec3 color, float intensity)
{
	this->type = type;
	this->color = color;
	this->intensity = intensity;
}

GrLight::GrLight(LIGHT_TYPES type, glm::vec3 color, float intensity, float constant, float linear, float quadratic)
{
	this->type = type;
	this->color = color;
	this->intensity = intensity;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}


GrLight::~GrLight()
{
}

void GrLight::useLight(Shader* shader)
{
	shader->setVec3("ambientLight.color", this->color);
	shader->setFloat("ambientLight.intensity", this->intensity);
}

GrLight* createAmbientLight(glm::vec3 color, float intensity)
{
	return new GrLight(LIGHT_TYPES::AMBIENT, color, intensity);
}

GrLight* createDirectionalLight(glm::vec3 color, float intensity, glm::vec3 direction)
{
	GrLight* light = new GrLight(LIGHT_TYPES::DIRECTIONAL, color, intensity);
	light->direction = direction;
	return light;
}

GrLight* createPointLight(glm::vec3 color, float intensity, glm::vec3 position, float constant, float linear, float quadratic)
{
	GrLight* light = new GrLight(LIGHT_TYPES::POINT, color, intensity);
	light->setPosition(position);
	light->constant = constant;
	light->linear = linear;
	light->quadratic = quadratic;
	return light;
}
