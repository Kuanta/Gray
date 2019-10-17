#include "grTexture.h"



grTexture::grTexture()
{
	this->textureID = 0;
	this->width = 0;
	this->height = 0;
	this->bitDepth = 0;

}


grTexture::~grTexture()
{
}

GLuint grTexture::getTextureID()
{
	return this->textureID;
}

void grTexture::useTexture(int textureOrder)
{
	glActiveTexture(GL_TEXTURE0 + textureOrder);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void grTexture::loadTexture(const char* filename)
{
	this->filename = string(filename);
	cout << "Filename:" << this->filename.c_str() << endl;
	unsigned char *texData = stbi_load(filename, &this->width, &this->height, &this->bitDepth,0);
	if (!texData)
	{
		printf("Failed to find :%s\n", filename);
		return;
	}

	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	cout << "Generated at :" << this->textureID << endl;
	//Texture Params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);
}

void grTexture::loadTexture(unsigned char * data, int width, int height)
{
	cout << "w:" << width << " h:" << height << endl;
	this->width = width;
	this->height = height;
	glGenTextures(1, &this->textureID);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	cout << "Generated at :" << this->textureID << endl;
	//Texture Params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void grTexture::clearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
}
