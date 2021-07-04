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
void GrLight::createDepthMap(unsigned int frameBuffer)
{
	if(this->type == LIGHT_TYPES::DIRECTIONAL)
	{
		// glGenTextures(1, &this->depthMap);
		// glBindTexture(GL_TEXTURE_2D_ARRAY, this->depthMap);
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// glGenFramebuffers(1, &frameBuffer);
		// glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		// glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthMap, 0);
	}
}

vector<glm::mat4> GrLight::getTransformMat(int width, int height)
{
	vector<glm::mat4> matrices;
	if(this->type == LIGHT_TYPES::DIRECTIONAL)
	{
		glm::vec3 targetVec = this->position + this->direction*1.0f;
		glm::mat4 lightViewMatrix = glm::lookAt(this->position, targetVec,glm::vec3(0.0f,1.0f,0.0f));
		glm::mat4 lightProjMatrix = glm::ortho(-40.0f,40.0f,-40.0f,40.0f,1.0f, 200.0f);
		matrices.push_back(lightProjMatrix*lightViewMatrix);
	}else
	{
		float aspect = (float)width / (float)height;
		glm::vec3 lightPos = this->position;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, 0.001f, this->farPlane);
		matrices.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
matrices.push_back(shadowProj * 
                 glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
matrices.push_back(shadowProj * 
                 glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
matrices.push_back(shadowProj * 
                 glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
matrices.push_back(shadowProj * 
                 glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
matrices.push_back(shadowProj * 
                 glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));
	}
	return matrices;
}

GrLight* createAmbientLight(glm::vec3 color, float intensity)
{
	return new GrLight(LIGHT_TYPES::AMBIENT, color, intensity);
}

GrLight* createDirectionalLight(glm::vec3 color, float intensity, glm::vec3 direction)
{
	GrLight* light = new GrLight(LIGHT_TYPES::DIRECTIONAL, color, intensity);
	light->direction = direction;
	light->requiresUpdate = true;
	return light;
}

GrLight* createPointLight(glm::vec3 color, float intensity, glm::vec3 position, float constant, float linear, float quadratic)
{
	GrLight* light = new GrLight(LIGHT_TYPES::POINT, color, intensity);
	light->setPosition(position);
	light->constant = constant;
	light->linear = linear;
	light->quadratic = quadratic;
	light->requiresUpdate = true;
	return light;
}
