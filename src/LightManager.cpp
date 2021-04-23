#include "LightManager.h"
#include "GameManager.h"
#include "Scene.h"

LightManager::LightManager()
{
	this->gm = nullptr;
	this->scene = nullptr;
}
LightManager::LightManager(GameManager* gm, Scene* scene)
{
	this->gm = gm;
	this->scene = scene;
	this->init();
}

LightManager::~LightManager()
{
}

void LightManager::init()
{
	//Create shadow map array
	glGenTextures(1,&this->shadowMaps);
	glBindTexture(GL_TEXTURE_2D, this->shadowMaps);

	//glTexStorage3D(GL_TEXTURE_2D_ARRAY,0,GL_DEPTH_COMPONENT,2048, 2048, 10);  //Support for up to 10 shadowmaps
	
	// glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	// glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 10);
	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, 2048, 2048,10, 0, GL_RGB, GL_FLOAT,0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 4096, 4096,0,GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->shadowMaps, 0);
	// GLenum DrawBuffers[10];
    // for (int i = 0; i < 10; ++i)
	// {
	// 	glFramebufferTextureLayer(GL_TEXTURE_2D_ARRAY, GL_COLOR_ATTACHMENT0, this->shadowMaps, 0, i);
	// 	DrawBuffers[i] = GL_COLOR_ATTACHMENT0+i; //Sets appropriate indices for each color buffer
	// }
   
	// glDrawBuffers(40, DrawBuffers);
	glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightManager::add(GrLight* light)
{
	this->lights.addElement(light);
	this->numberOfLights++;
}

void LightManager::remove(GrLight* light)
{
	this->lights.removeElement(light);
	this->numberOfLights--;
}

void LightManager::update(float delta)
{
}

void LightManager::draw()
{
	int counter = 0;
	int shadowCounter = 0;
	for (vector<GrLight*>::iterator it = this->lights.elements.begin(); it != this->lights.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			GrLight* light = *it;
			this->lightStructs[counter].type = (int)light->type;
			this->lightStructs[counter].transformMat = light->getTransformMat();
			this->lightStructs[counter].color = glm::vec4(light->color,1);
			this->lightStructs[counter].position = glm::vec4(light->getPosition(),1);
			this->lightStructs[counter].direction = glm::vec4(light->direction,1);
			this->lightStructs[counter].intensity = light->intensity;
			this->lightStructs[counter].constant = light->constant;
			this->lightStructs[counter].linear = light->linear;
			this->lightStructs[counter].quadratic = light->quadratic;
			this->lightStructs[counter].castShadow = light->castShadow;

			//Create Shadow Map if light casts shadow
			if(light->castShadow)
			{
				//Draw the scene
				Shader* depthShader = this->gm->shaderMan.getShaderByType(SHADER_TYPE::DEPTH_SHADER);
				glViewport(0,0,4096, 4096); //Shadow width and height
				glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
				glClear(GL_DEPTH_BUFFER_BIT);

				depthShader->use();
				depthShader->setMat4("lightMatrix", light->getTransformMat());
				this->scene->drawElements(depthShader);
				
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, this->gm->windowWidth, this->gm->windowHeight);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				shadowCounter++;
			}
			counter++;

			
		}
	}

	this->lightsBuffer->bindBuffer();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 100*144, this->lightStructs);
	glBufferSubData(GL_UNIFORM_BUFFER, 100*144, 4, &this->numberOfLights); //Send the number of lights
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //Unbind
	//Activate the texture for shadowmaps
	glActiveTexture(GL_TEXTURE0+4);
	glBindTexture(GL_TEXTURE_2D, this->shadowMaps);
	this->scene->activateFbo();

}

void LightManager::clean()
{

}

void LightManager::updateBuffer()
{
	//Number of bytes for a single Light structure in the shader is 
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, this->numberOfLights*11*4, reinterpret_cast<>);
}

void LightManager::activateShadowMap(int layer)
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->shadowMaps);
}