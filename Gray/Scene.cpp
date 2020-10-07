#include "Scene.h"
#include "GameManager.h"


Scene::Scene(GameManager * gm)
{
	this->camera = Camera();
	this->gm = gm;
	this->window = gm->window;
	this->matricesBuffer = nullptr;
}

Scene::~Scene()
{

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	gm->getShader(SHADER_TYPE::DEFAULT_SHADER)->setMat4("view", this->camera.getViewMatrix());
	gm->getShader(SHADER_TYPE::DEFAULT_SHADER)->setMat4("projection", this->camera.projection);
	gm->getShader(SHADER_TYPE::DEFAULT_SHADER)->setVec3("eyePosition", this->camera.getPosition());

	//Send Matrices to the buffer
	if (this->matricesBuffer != nullptr)
	{
		this->matricesBuffer->bindBuffer(); //Set the active buffer
		this->matricesBuffer->updateMat4(this->camera.getViewMatrix(), 0);
		this->matricesBuffer->updateMat4(this->camera.projection, sizeof(glm::mat4));
		this->matricesBuffer->updateVec3(this->camera.getPosition(), sizeof(glm::mat4) * 2);
	}

	//Draw lights
	this->lm.draw();

	//Draw each element
	for (vector<Object*>::iterator it = this->em.elements.begin(); it != this->em.elements.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->draw();
		}

	}
}
