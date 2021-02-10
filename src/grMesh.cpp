#include "grMesh.h"
#include "Material.h"


GrMesh::GrMesh(Geometry * geometry, Material * material)
{
	this->geometry = geometry;
	this->material = material;
	this->VAO = geometry->VAO;
	this->VBO = geometry->VBO;
	this->EBO = geometry->EBO;
	
}

GrMesh::GrMesh(Geometry * geometry, Material * material, string name)
{
	this->geometry = geometry;
	this->material = material;
	this->VAO = geometry->VAO;
	this->VBO = geometry->VBO;
	this->EBO = geometry->EBO;
	this->name = name;
}

GrMesh::~GrMesh()
{
}

void GrMesh::start()
{
	this->material->gm = this->object->gm;
}

void GrMesh::update(float deltaTime)
{
	
}

void GrMesh::draw(Shader* shader)
{
	//if (this->object->getRootObject()->gm != nullptr)
	if(shader != nullptr)
	{
		//Shader* shader = this->object->getRootObject()->gm->shaders.defaultShader;
		shader->use();
		//Set Model-View_Projection
		shader->setMat4("model", this->object->model);
		//shader->setMat4("view", this->object->getRootObject()->gm->currentScene->camera.getViewMatrix());
		//shader->setMat4("projection", this->object->getRootObject()->gm->currentScene->camera.projection);
		//BUG FIX: MATERIAL SHOULD BE DRAWN FIRST
		this->material->draw(shader);
		this->geometry->draw(shader);	
	}
}

void GrMesh::cleanup()
{
	this->material->cleanup();
	this->geometry->cleanup();
}

Component* GrMesh::clone()
{
	Geometry* geo = this->geometry->clone();
	Material* mat = this->material->clone();
	GrMesh* cloned = new GrMesh(geo, mat);

	return cloned;
}


