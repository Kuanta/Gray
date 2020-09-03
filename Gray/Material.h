#ifndef MATERIAL_H
#define MATERIAL_H
#include <iostream>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "grTexture.h"
#include "GameManager.h"

struct Texture {
	unsigned int id;
	int textureNumber;
	aiTextureType type;
	aiString path;
};

enum TextureTypes { DIFFUSE_TEXTURE, SPECULAR_TEXTURE, NORMAL_TEXTURE, HEIGHT_TEXTURE };

class Material
{
public:
	Material();
	Material(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, float shininess);
	~Material();
	

	//Getters and Setters
	void setTexture(const char* filename, TextureTypes textureType);
	void setTexture(grTexture* texture, TextureTypes textureType);
	grTexture* getTexture(TextureTypes textureType);
	void draw(Shader* shader);
	Material* clone();
	void cleanup();

	//Uniforms
	GameManager* gm = nullptr;

	//Blinn-Phong
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;
	float shininess = 32;
	float diffuseIntensity = 1;
	float ambientIntensity = 1;
	float specularIntensity = 1;

	//PBR
	bool metalness;
	float rougness;
	glm::vec3 color;
private:
	//Textures
	grTexture* diffuseMap;
};

#endif
