#pragma once
#ifndef Material_H
#define Material_H
#include <iostream>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "grTexture.h"


struct Texture {
	unsigned int id;
	int textureNumber;
	aiTextureType type;
	aiString path;
};

enum TextureTypes { DIFFUSE_TEXTURE, SPECULAR_TEXTURE, NORMAL_TEXTURE, HEIGHT_TEXTURE, PBR_TEXTURE, DISP_TEXTURE };

class Material
{
public:
	Material();
	Material(glm::vec3 color, float shininess, float metalness, float rougness);
	~Material();
	

	//Getters and Setters
	void initTextures();
	void setTexture(const char* filename, TextureTypes textureType);
	void setTexture(grTexture* texture, TextureTypes textureType);
	grTexture* getTexture(TextureTypes textureType);
	void draw(Shader* shader);
	Material* clone();
	void cleanup();

	//Blinn-Phong
	glm::vec3 color;
	float shininess = 32;

	//PBR
	float metalness;
	float rougness;

	//General Properties
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;

private:
	//Textures
	grTexture* diffuseMap;
	grTexture* normalMap;
	
	grTexture* pbrMap; //Roughness and metalness combined
	grTexture* dispMap;
};

#endif
