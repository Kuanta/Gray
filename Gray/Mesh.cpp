#include "Mesh.h"

Mesh::Mesh(Geometry * geometry, Material * material)
{
	this->geometry = geometry;
	this->material = material;
	this->VAO = geometry->VAO;
	this->VBO = geometry->VBO;
	this->EBO = geometry->EBO;
	
}

Mesh::~Mesh()
{
}

void Mesh::start()
{
	this->material->gm = this->object->gm;
}

void Mesh::update(float deltaTime)
{
	
}

void Mesh::draw()
{
	if (this->object->getRootObject()->gm != nullptr)
	{
		Shader* shader = this->object->getRootObject()->gm->shaders.defaultShader;
		shader->use();
		//Set Model-View_Projection
		shader->setMat4("model", this->object->model);
		shader->setMat4("view", this->object->getRootObject()->gm->currentScene->camera.getViewMatrix());
		shader->setMat4("projection", this->object->getRootObject()->gm->currentScene->camera.projection);
		//BUG FIX: MATERIAL SHOULD BE DRAWN FIRST
		this->material->draw(shader);
		this->geometry->draw(shader);	
	}
}

void Mesh::cleanup()
{
	this->material->cleanup();
	this->geometry->cleanup();
}


