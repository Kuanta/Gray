#include "Material.h"

Material::Material()
{
	//glm variables initiates themselves
	this->diffuse = glm::vec3(0);
	this->shininess = 0;
	this->metalness = 0.0f;
	this->rougness = 0.15f;
	this->initTextures();
}

Material::Material(glm::vec3 color, float shininess, float metalness, float rougness)
{
	this->diffuse = color;
	this->shininess = shininess;
	this->metalness = metalness;
	this->rougness = rougness;
	this->initTextures();
}


Material::~Material()
{
	
}

void Material::initTextures()
{
	this->diffuseMap = nullptr;
	this->normalMap = nullptr;
	this->pbrMap = nullptr;
	this->dispMap = nullptr;
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
		case HEIGHT_TEXTURE:
		case NORMAL_TEXTURE:
			if(this->normalMap != nullptr){
				this->normalMap->clearTexture();
				delete this->normalMap;
			}
			this->normalMap = new grTexture();
			this->normalMap->loadTexture(filename);
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
		case HEIGHT_TEXTURE:
		case NORMAL_TEXTURE:
			if (this->normalMap != nullptr)
			{
				this->normalMap->clearTexture();
				delete this->normalMap;
			}
			this->normalMap = texture;
			break;
		case PBR_TEXTURE:
			if (this->pbrMap != nullptr)
			{
				this->pbrMap->clearTexture();
				delete this->pbrMap;
			}
			this->pbrMap = texture;
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
		break;
	case HEIGHT_TEXTURE:
	case NORMAL_TEXTURE:
		return this->normalMap;
		break;
	case PBR_TEXTURE:
	return this->pbrMap;
	default:
		break;
	}
	return nullptr;
}

void Material::draw(Shader* shader)
{
	shader->use();
	
	
	if (this->diffuseMap != nullptr && this->diffuseMap->textureExists()) {
		//Diffuse (Using first 5 maps for shadows)
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->diffuseMap->getTextureID());
		shader->setInt("diffuseMap", 0);
		shader->setBool("containsTexture", 1);
		this->diffuseMap->useTexture(0);
	}
	else {
		shader->setBool("containsTexture", 0);
	}
	
	//Normal
	if (this->normalMap != nullptr && this->normalMap->textureExists())
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, this->normalMap->getTextureID());
		shader->setInt("normalMap", 1);
		shader->setBool("containsNormalTexture", 1);
		this->normalMap->useTexture(1);
	}
	else
	{
		shader->setBool("containsNormalTexture", 0);
	}

	//Roughness
	if(this->pbrMap != nullptr && this->pbrMap->textureExists())
	{
		glActiveTexture(GL_TEXTURE0+3);
		glBindTexture(GL_TEXTURE_2D, this->pbrMap->getTextureID());
		shader->setInt("pbrMap", 3);
		shader->setBool("containsPbrTexture", 1);
	}else{
		shader->setBool("containsPbrTexture", 0);
	}
	//Set uniforms
	shader->setVec3("material.color", this->diffuse);
	shader->setFloat("material.shininess", this->shininess);
	shader->setFloat("material.metal", this->metalness);
	shader->setFloat("material.roughness", this->rougness);
}

Material* Material::clone()
{
	return this;
}

void Material::cleanup()
{
	this->diffuseMap->clearTexture();
}
