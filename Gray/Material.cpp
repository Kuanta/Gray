#include "Material.h"



Material::Material()
{
	//glm variables initiates themselves
	this->shininess = 0;
	this->diffuseMap = nullptr;
	this->diffuse = glm::vec3(0.5, 0.5, 0.5);
	this->ambient = glm::vec3(0.5, 0.5, 0.5);
	this->specular = glm::vec3(0.5, 0.5, 0.5);
	this->shininess = 0.5;
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

void Material::setTexture(const char * filename, TextureTypes textureType)
{
	
	switch (textureType){
		case DIFFUSE_TEXTURE:
			//Clear diffusemap first
			if (this->diffuseMap != nullptr) {
				this->diffuseMap->clearTexture();
				delete this->diffuseMap;
			}
			this->diffuseMap = new grTexture();
			this->diffuseMap->loadTexture(filename);
			break;
		default:
			break;
	}
	
}

void Material::setTexture(grTexture * texture, TextureTypes textureType)
{
	switch (textureType) {
		case DIFFUSE_TEXTURE:
			//Clear diffusemap first
			if (this->diffuseMap != nullptr) {
				this->diffuseMap->clearTexture();
				delete this->diffuseMap;
			}
			this->diffuseMap = texture;
			break;
		default:
			break;
	}
}

grTexture * Material::getTexture(TextureTypes textureType)
{
	switch (textureType) {
	case DIFFUSE_TEXTURE:
		return this->diffuseMap;
	default:
		break;
	}
	return nullptr;
}

void Material::draw(Shader* shader)
{
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

Material* Material::clone()
{
	Material* cloned = new Material();
	cloned->gm = this->gm;
	cloned->diffuse = this->diffuse;
	cloned->ambient = this->ambient;
	cloned->specular = this->specular;
	cloned->shininess = this->shininess;
	cloned->diffuseIntensity = this->diffuseIntensity;
	cloned->ambientIntensity = this->ambientIntensity;
	cloned->specularIntensity = this->specularIntensity;
	cloned->setTexture(this->diffuseMap, TextureTypes::DIFFUSE_TEXTURE);
	return cloned;
}

void Material::cleanup()
{
	this->diffuseMap->clearTexture();
}
