#pragma once
#include <glad\glad.h>
#include <iostream>
#include "stb_image.h"

using namespace std;

class grTexture
{
public:
	grTexture();
	~grTexture();

	//Getters & Setters
	void setFilename(const char* filename) {
		this->filename = filename;
	}
	string getFilename()
	{
		return string(this->filename);
	}
	GLuint getTextureID();
	bool textureExists() {
		if (width > 0 && height > 0) {
			return true;
		}
		else {
			return false;
		}
	}
	void useTexture(int textureOrder);
	void loadTexture(const char* filename);
	void loadTexture(unsigned char* data, int width, int height);
	void clearTexture();
private:
	int width, height, bitDepth;
	GLuint textureID;
	string filename;
};

