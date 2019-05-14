#pragma once
#include <GL/glew.h>
#include "../Texture/Texture.h"
#include "../Strutture/MeshStruct.h"
#include "../Shader/ShaderLoader.h"
#include "../Camera/Camera.h"

class Skybox {
private:
	GLuint VAO, VBO, skyboxTexture;

public:
	Skybox() {};
	Skybox(SkyboxStruct Skybox);
	void Draw(ShaderLoader shader, Camera &camera);
	void DeleteSkybox();
};