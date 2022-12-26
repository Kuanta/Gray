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

	glGenFramebuffers(1, &this->depthMapFBO);
	glGenFramebuffers(1, &this->depthMapFBOCube);
	glGenFramebuffers(1, &this->depthMapFBOStatic);
	glGenFramebuffers(1, &this->depthMapFBOCubeStatic);
	this->depthMap = this->createDepthMap(this->depthMapFBO, this->depthMap);
	this->depthMapStatic = this->createDepthMap(this->depthMapFBOStatic, this->depthMapStatic);
	this->depthCubeMap = this->createCubeMap(this->depthMapFBOCube, this->depthCubeMap);
	this->depthCubeMapStatic = this->createCubeMap(this->depthMapFBOCubeStatic, this->depthCubeMapStatic);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightManager::add(GrLight* light)
{
	this->lights.addElement(light);
	this->numberOfLights++;
	light->createDepthMap(this->depthMapFBO);
}

void LightManager::remove(GrLight* light)
{
	this->lights.removeElement(light);
	this->numberOfLights--;
}

void LightManager::update(float delta)
{
}

unsigned int LightManager::createDepthMap(unsigned int fbo, unsigned int depthMap)
{
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32, this->shadowDepth, this->shadowHeight, this->MAX_DIRECT_LIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	return depthMap;
}
unsigned int LightManager::createCubeMap(unsigned int fbo, unsigned int cubeMap)
{
	glGenTextures(1, &cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, cubeMap);
	glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT32, this->pointShadowDepth, this->pointShadowHeight, this->MAX_POINT_LIGHT * 6, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //linear
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //linear

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	return cubeMap;
}
void LightManager::draw()
{


	int pointLightCounter = 0;
	int directLightCounter = 0;
	int counter = 0;
	for (vector<GrLight*>::iterator it = this->lights.elements.begin(); it != this->lights.elements.end(); it++)
	{

		if ((*it) != nullptr)
		{
			GrLight* light = *it;
			this->lightStructs[counter].type = (int)light->type;
			this->lightStructs[counter].transformMat = light->getTransformMat(this->shadowDepth, this->shadowHeight)[0];
			this->lightStructs[counter].color = glm::vec4(light->color,1);
			this->lightStructs[counter].position = glm::vec4(light->getPosition(),1);
			this->lightStructs[counter].direction = glm::vec4(light->direction,1);
			this->lightStructs[counter].intensity = light->intensity;
			this->lightStructs[counter].constant = light->constant;
			this->lightStructs[counter].linear = light->linear;
			this->lightStructs[counter].quadratic = light->quadratic;
			this->lightStructs[counter].castShadow = light->castShadow;
			this->lightStructs[counter].far_plane = light->farPlane;
			this->lightStructs[counter].attenuationFactor = light->attenuationFactor;
			this->lightStructs[counter].softnessFactor = light->softShadowFactor;

			//Create Shadow Map if light casts shadow
			if(light->castShadow)
			{
				//Draw the scene
				Shader* depthShader;
	
				vector<glm::mat4> lightMatrices;
				int index;
				unsigned int staticFbo;
				unsigned int dynamicFbo;
				//lightMatrices = light->getTransformMat(this->shadowDepth, this->shadowHeight);
				if(light->type == LIGHT_TYPES::DIRECTIONAL)
				{
					depthShader = this->gm->shaderMan.getShaderByType(SHADER_TYPE::DEPTH_SHADER);
					index = directLightCounter;
					staticFbo = this->depthMapFBOStatic;
					dynamicFbo = this->depthMapFBO;
				}
				else if (light->type == LIGHT_TYPES::POINT)
				{
					index = pointLightCounter;
					depthShader = this->gm->shaderMan.getShaderByType(SHADER_TYPE::DEPTH_POINT_SHADER);
					staticFbo = this->depthMapFBOCubeStatic;
					dynamicFbo = this->depthMapFBOCube;
				}
				
				if(light->requiresUpdate)
				{
					this->prepareDrawing(light, staticFbo, true, index);
					this->scene->drawStaticElements(light, depthShader);
					light->requiresUpdate = false;
				}

				//Draw dynamics
				if(!light->staticLight)
				{
					//Static lights won't update their maps even for dynamic objects
					this->prepareDrawing(light, dynamicFbo, false, index);
					this->scene->drawDynamicElements(light, depthShader);
				}
			}
			if(light->type == LIGHT_TYPES::DIRECTIONAL)
			{
				directLightCounter++;
			}else if (light->type == LIGHT_TYPES::POINT)
			{
				pointLightCounter++;
			}
			counter++;
		}
	}
	this->lightsBuffer->bindBuffer();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 100*160, this->lightStructs);
	glBufferSubData(GL_UNIFORM_BUFFER, 100*160, 4, &this->numberOfLights); //Send the number of lights
	glBindBuffer(GL_UNIFORM_BUFFER, 0); //Unbind,

	//Activate the texture for shadowmaps
	glViewport(0, 0, this->gm->windowWidth, this->gm->windowHeight);
	this->scene->activateFbo();
}
void LightManager::prepareDrawing(GrLight *light, unsigned int fbo, bool isStatic, int index)
{
	/*
	Prepares fbo for drawing. Assigns correct depth buffers
	light: Light to do calculations for
	isStatic: If set to true, depth buffer for static objects will be attached
	index: Index of the light for layer number
	*/
	vector<glm::mat4> lightMatrices;
	
	Shader *depthShader;


	if (light->type == LIGHT_TYPES::DIRECTIONAL)
	{
		lightMatrices = light->getTransformMat(this->shadowDepth, this->shadowHeight);
		glViewport(0, 0, this->shadowDepth, this->shadowHeight); //Shadow width and height
		depthShader = this->gm->shaderMan.getShaderByType(SHADER_TYPE::DEPTH_SHADER);
		depthShader->use();
		if(isStatic)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBOStatic);
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthMapStatic, 0, index); //This is the important thing
			glClear(GL_DEPTH_BUFFER_BIT);
		}else{
			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthMap, 0, index); //This is the important thing
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		depthShader->setMat4("lightMatrix", light->getTransformMat(this->shadowDepth, this->shadowHeight)[0]);
	}
	else if (light->type == LIGHT_TYPES::POINT)
	{
		lightMatrices = light->getTransformMat(this->pointShadowDepth, this->pointShadowHeight);
		glViewport(0, 0, this->pointShadowDepth, this->pointShadowHeight); //Shadow width and height
		depthShader = this->gm->shaderMan.getShaderByType(SHADER_TYPE::DEPTH_POINT_SHADER);
		depthShader->use();
		if(isStatic)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBOCubeStatic);
			//Clear specific faces
			for(int i=0;i<6;i++)
			{
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubeMapStatic, 0, index * 6+i);
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubeMapStatic, 0); //Attach whole map again
		}else{
			glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBOCube);
			//Clear specific layers
			for (int i = 0; i < 6; i++)
			{
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubeMap, 0, index * 6 + i);
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthCubeMap, 0); //Attach whole map again
		}
		glUniformMatrix4fv(glGetUniformLocation(depthShader->ID, "lightMatrix"), lightMatrices.size(), GL_FALSE, glm::value_ptr(lightMatrices[0]));
		depthShader->setFloat("far_plane", light->farPlane);
		depthShader->setVec3("lightPos", light->getPosition());
		glm::vec3 pos = light->getPosition();
		depthShader->setInt("index", index);
	}

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
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
}