#include "Material.h"

Material::Material()
{
	//glm variables initiates themselves
	this->color = glm::vec3(0);
	this->shininess = 0;
	this->metalness = false;
	this->rougness = 0.15f;
	this->diffuseMap = nullptr;
}

Material::Material(glm::vec3 color, float shininess, bool metalness, float rougness)
{
	this->color = color;
	this->shininess = shininess;
	this->metalness = metalness;
	this->rougness = rougness;
	this->diffuseMap = nullptr;
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
	glActiveTexture(GL_TEXTURE0 + 5);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 1);
	if (this->diffuseMap != nullptr && this->diffuseMap->textureExists()) {
		shader->setBool("containsTexture", 1);
		this->diffuseMap->useTexture(0);
	}
	else {
		shader->setBool("containsTexture", 0);
	}

	//Set uniforms
	shader->setVec3("material.color", this->diffuse);
	shader->setFloat("material.shininess", this->shininess);
	shader->setBool("material.metal", this->metalness);
	shader->setFloat("material.roughness", this->rougness);
}

Material* Material::clone()
{
	Material* cloned = new Material();
	cloned->gm = this->gm;
	cloned->shininess = this->shininess;
	cloned->setTexture(this->diffuseMap, TextureTypes::DIFFUSE_TEXTURE);
	return cloned;
}

void Material::cleanup()
{
	this->diffuseMap->clearTexture();
}
