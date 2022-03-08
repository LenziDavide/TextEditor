#pragma once

#include <GLFW/glfw3.h>

class Texture
{
public:
	static bool LoadFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
};