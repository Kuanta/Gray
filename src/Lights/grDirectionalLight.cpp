#include "Lights/grDirectionalLight.h"

grDirectionalLight::grDirectionalLight():GrLight()
{
	this->direction = glm::vec3(0,-1,0);
}

grDirectionalLight::grDirectionalLight(glm::vec3 color, float intensity, glm::vec3 direction):GrLight(LIGHT_TYPES::DIRECTIONAL, color, intensity)
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

glm::mat4 grDirectionalLight::getTransformMat()
{
	glm::mat4 lightViewMatrix = glm::lookAt(this->getPosition(), this->direction,glm::vec3(0.0f,1.0f,0.0f));
    glm::mat4 lightProjMatrix = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,1.0f, 100.0f);
	cout<<glm::to_string(lightProjMatrix*lightViewMatrix)<<endl;
	return lightProjMatrix*lightViewMatrix;
}