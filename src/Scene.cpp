#include "Scene.h"
#include "GameManager.h"
#include "Material.h"


Scene::Scene(GameManager * gm)
{
	//this->camera = Camera();
	this->gm = gm;
	this->window = gm->window;
	this->matricesBuffer = nullptr;
	this->lm = LightManager(gm, this);
	this->keyInputCallback=nullptr;
	this->characterCallback=nullptr;
	this->charmodsCallback= nullptr;
	this->cursorPositionCallback=nullptr;
	this->cursorEnterCallback = nullptr;
	this->mouseButtonCallback = nullptr;
	this->scrollCallback = nullptr;
	this->dropCallback = nullptr;
	this->frameBufferSizeCallback = nullptr;

	//Init camera
	this->camera = Camera(75, this->gm->windowWidth / this->gm->windowHeight, 0.1f, 10000.0f);
	this->camera.setPositionY(8);
	this->camera.setTarget(glm::vec3(0.0f, 0.0f, -1.0f));

	//Init gbuffer
	this->gbuffer = new Gbuffer(this->gm->windowWidth, this->gm->windowHeight);
	//Init deferred Canvas
	this->deferredCanvas = new Object();
	GrMesh *deferredCanvasMesh;
	Plane *pg = new Plane(2.0f, 2.0f);
	Material *mat = new Material(glm::vec3(1.0f, 1.0f, 1.0f), 1, 1, 1);
	deferredCanvasMesh = new GrMesh(pg, mat);
	this->deferredCanvas->addComponent(deferredCanvasMesh);
}

Scene::~Scene()
{

}

void Scene::init()
{
	this->initFbo();
}

void Scene::add(Object* object)
{
	this->em.addElement(object);
}

void Scene::add(GrLight* light)
{
	this->lm.add(light);
}

void Scene::remove(Object* object)
{
	this->em.removeElement(object);
}

void Scene::remove(GrLight* light)
{
	this->lm.remove(light);
}

void Scene::fixedUpdate()
{
}

void Scene::earlyUpdate(double deltaTime)
{
	for (vector<Object *>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->earlyUpdate(deltaTime);
		}
	}
}
void Scene::lateUpdate(double deltaTime)
{
	for (vector<Object *>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->lateUpdate(deltaTime);
		}
	}
}
void Scene::update(GLFWwindow* window, double deltaTime)
{
	this->camera.controls->update(deltaTime);
	for (vector<Object*>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->update(deltaTime);
		}
	}
	this->camera.getViewMatrix();
	this->draw(deltaTime);
	this->em.clearElements();
}

void Scene::draw(double deltaTime)
{
	//Enable canvas fbo
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	//Draw the scene
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//Send Matrices to the buffer
	if (this->matricesBuffer != nullptr)
	{
		this->matricesBuffer->bindBuffer(); //Set the active buffer
		this->matricesBuffer->updateMat4(this->camera.getViewMatrix(), 0);
		this->matricesBuffer->updateMat4(this->camera.projection, sizeof(glm::mat4));
		this->matricesBuffer->updateVec3(this->camera.getPosition(), sizeof(glm::mat4) * 2);
	}

	//Pre forward pass

	//Geometry Pass
	this->lm.draw();
	this->gbuffer->setActive();
	this->drawElements();
	this->lightPass();
	//Draw the canvas
	this->drawFbo();
}

void Scene::activateFbo()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}
void Scene::drawElements(Shader* shader)
{
	//Draw each element
	for (vector<Object*>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			if(shader != nullptr)
			{
				(*it)->draw(shader);  //For shadowbox method, a simple shader that only draw to depth map will be used for all the objects in the scene
			}
			else{
				(*it)->draw();
			}
		}

	}
}
void Scene::drawStaticElements(Shader* shader, bool lightFilter)
{
	//Draw each element
	for (vector<Object *>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		//TODO: Implement shadow casting on and off
		// if (lightFilter && !(*it)->castShadow)
		// {
		// 	//Don't show it in the depth buffer
		// 	continue;
		// }
		if ((*it) != nullptr && (*it)->isStatic && (*it)->castShadow)
		{
			if (shader != nullptr)
			{

				(*it)->draw(shader); //For shadowbox method, a simple shader that only draw to depth map will be used for all the objects in the scene
			}
			else
			{
				(*it)->draw();
			}
		}
	}
}

void Scene::drawDynamicElements(Shader* shader, bool lightFilter)
{
	//Draw each element
	for (vector<Object *>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		// if(lightFilter && !(*it)->castShadow)
		// {
		// 	//Don't show it in the depth buffer
		// 	continue;
		//}
		if ((*it) != nullptr && !(*it)->isStatic)
		{
			if (shader != nullptr)
			{
				(*it)->draw(shader); //For shadowbox method, a simple shader that only draw to depth map will be used for all the objects in the scene
			}
			else
			{
				(*it)->draw();
			}
		}
	}
}
void Scene::initFbo()
{
	/*
	*	This function is for demo purposes. It initializes a framebuffer alongside with a texture.
	*/
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glGenTextures(1, &fboTexture);
	glBindTexture(GL_TEXTURE_2D, fboTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,this->gm->windowWidth,this->gm->windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);  //TODO: Get width and height from gm

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Bind the texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fboTexture, 0);

	//Assign a render object to frame buffer for stencil and depth testing
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->gm->windowWidth, this->gm->windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "ERROR: Framebuffer is not complete" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->fboCanvas = new Object();
	GrMesh* fboCanvasMesh;
	Plane* pg = new Plane(2.0f, 2.0f);
	Material* mat = new Material(glm::vec3(0.5f, 0.5f, 0.5f), 1, true, 1);
	fboCanvasMesh = new GrMesh(pg, mat);
	this->fboCanvas->addComponent(fboCanvasMesh);
}

void Scene::drawFbo()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0); //Enable the default fbo
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	

	Shader* shader = this->gm->shaderMan.getShaderByType(SHADER_TYPE::FBO_SHADER);
	shader->use();
	shader->setFloat("postProcessFactor", this->postProcessFactor);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->fboTexture);

	this->fboCanvas->draw(shader);
}

void Scene::lightPass()
{
	this->lm.draw();

	Shader* deferredShader = this->gm->shaderMan.getShaderByType(SHADER_TYPE::DEFERRED_SHADER);
	deferredShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->gbuffer->gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->gbuffer->gNormals);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->gbuffer->gColors);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->gbuffer->gPbr);

	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->lm.depthMap);
	glActiveTexture(GL_TEXTURE0 + 6);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, this->lm.depthCubeMap);
	glActiveTexture(GL_TEXTURE0 + 7);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->lm.depthMapStatic);
	glActiveTexture(GL_TEXTURE0 + 8);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, this->lm.depthCubeMapStatic);
	this->deferredCanvas->draw(deferredShader);
}

void Scene::cleanup()
{
	
}