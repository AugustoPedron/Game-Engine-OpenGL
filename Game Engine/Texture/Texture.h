#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include "../SOIL2/SOIL2.h"

class TextureLoader {
public:
	static GLuint LoadTexture(const GLchar *path);
	static GLuint LoadSkyboxMap(std::vector<const GLchar *> faces);
};
