#pragma once

#include <GL/glew.h>

#include "stb_image.h"

#include <iostream>
class Texture
{
public:
	Texture();
	Texture(const char* path);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureId;
	int width, height, bitDepth;
	const char* fileLocation;
};

