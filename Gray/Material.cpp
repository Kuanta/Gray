#include "Material.h"



Material::Material()
{
	//glm variables initiates themselves
	this->shininess = 0;
	this->diffuseMap = nullptr;
}

Material::Material(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, float shininess)
{
	this->diffuse = diffuse;
	this->ambient = ambient;
	this->specular = specular;
	this->shininess = shininess;
}


Material::~Material()
{
}

void Material::draw()
{
	if (this->gm != nullptr)
	{
		Shader* shader = this->gm->shaders.defaultShader;
		shader->use();
		if (this->diffuseMap != nullptr && this->diffuseMap->textureExists()) {
			shader->setBool("containsTexture", 1);
			this->diffuseMap->useTexture(0);
		}
		else {
			shader->setBool("containsTexture", 0);
		}
		
		//Set uniforms
		shader->setVec3("material.diffuse", this->diffuse);
		shader->setVec3("material.ambient", this->ambient);
		shader->setVec3("material.specular", this->specular);
		shader->setFloat("material.shininess", this->shininess);
	}
}

void Material::setDiffuseTexture(char * filename)
{
	//Clear diffusemap first
	if (this->diffuseMap != nullptr) {
		this->diffuseMap->clearTexture();
		delete this->diffuseMap;
	}
	this->diffuseMap = new grTexture();
	this->diffuseMap->loadTexture(filename);
}

void Material::cleanup()
{
	this->diffuseMap->clearTexture();
}
