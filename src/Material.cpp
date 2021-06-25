#include "Material.h"

Material::Material()
{
	//glm variables initiates themselves
	this->color = glm::vec3(0);
	this->shininess = 0;
	this->metalness = false;
	this->rougness = 0.15f;
	this->initTextures();
}

Material::Material(glm::vec3 color, float shininess, bool metalness, float rougness)
{
	this->color = color;
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
	this->roughnessMap = nullptr;
	this->metalMap = nullptr;
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
		glActiveTexture(GL_TEXTURE0 );
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->diffuseMap->getTextureID());
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
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->normalMap->getTextureID());
		shader->setInt("normalMap", 1);
		shader->setBool("containsNormalTexture", 1);
		this->normalMap->useTexture(1);
	}
	else
	{
		shader->setBool("containsNormalTexture", 0);
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
