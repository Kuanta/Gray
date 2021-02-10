#include "Lights/grLight.h"



GrLight::GrLight()
{
	this->type = LIGHT_TYPES::AMBIENT;
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);
	this->intensity = 1.0f;
	this->castShadow = 0;
}

GrLight::GrLight(LIGHT_TYPES type)
{
	this->type = type;
	this->castShadow = 0;
}

GrLight::GrLight(LIGHT_TYPES type, glm::vec3 color, float intensity)
{
	this->type = type;
	this->color = color;
	this->intensity = intensity;
	this->castShadow = 0;
}

GrLight::GrLight(LIGHT_TYPES type, glm::vec3 color, float intensity, float constant, float linear, float quadratic)
{
	this->type = type;
	this->color = color;
	this->intensity = intensity;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->castShadow = 0;
}


GrLight::~GrLight()
{
}

void GrLight::useLight(Shader* shader)
{
	shader->setVec3("ambientLight.color", this->color);
	shader->setFloat("ambientLight.intensity", this->intensity);
}

glm::mat4 GrLight::getTransformMat()
{
	if(this->type == LIGHT_TYPES::DIRECTIONAL)
	{
		glm::vec3 targetVec = this->position + this->direction*1.0f;
		glm::mat4 lightViewMatrix = glm::lookAt(this->position, targetVec,glm::vec3(0.0f,1.0f,0.0f));
		glm::mat4 lightProjMatrix = glm::ortho(-40.0f,40.0f,-40.0f,40.0f,1.0f, 200.0f);
		return lightProjMatrix*lightViewMatrix;
	}else
	{
		return glm::mat4(1.0f);
	}
	
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
