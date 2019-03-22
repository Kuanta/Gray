#pragma once
#include "stb_image.h"
#include <glad\glad.h>

class grTexture
{
public:
	grTexture();
	~grTexture();

	//Getters & Setters
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
	void clearTexture();
private:
	int width, height, bitDepth;
	GLuint textureID;
	const char* filename;
};

