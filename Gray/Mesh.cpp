#include "Mesh.h"


Mesh::Mesh(Geometry * geometry, Material * material)
{
	this->geometry = geometry;
	this->material = material;
	this->VAO = geometry->VAO;
	this->VBO = geometry->VBO;
	this->EBO = geometry->EBO;
	this->parent = nullptr;
	this->scale = glm::vec3(1,1,1);
	this->material->gm = this->gm;
}

Mesh::~Mesh()
{
}

void Mesh::update(float deltaTime)
{
	Object::update(deltaTime);
}

void Mesh::draw()
{
	this->updateModel();

	if (this->gm != nullptr)
	{
		Shader* shader = this->gm->shaders.defaultShader;
		shader->use();
		//Set Model-View_Projection
		shader->setMat4("model", this->model);
		shader->setMat4("view", this->gm->currentScene->camera.getViewMatrix());
		shader->setMat4("projection", this->gm->currentScene->camera.projection);
		this->geometry->draw();
		this->material->draw();
	}
	Object::draw();
	
}

void Mesh::cleanup()
{
	this->material->cleanup();
	this->geometry->cleanup();
}


