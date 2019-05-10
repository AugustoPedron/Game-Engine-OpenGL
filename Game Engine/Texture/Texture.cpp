#include "Texture.h"
//si occupa del caricamento delle texture da un file di immagine
GLuint TextureLoader::LoadTexture(const GLchar *path) {
	//viene generato un textureID che è un array di grandezza specificata nel glGenTextures
	GLuint textureId;
	glGenTextures(1, &textureId);
	int textureWidth, textureHeight;
	unsigned char* image;

	image = SOIL_load_image(path, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	if (image) {
		//viene specificato a quale indice stiamo assegnando la texture caricata
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		std::cout << "Errore nel caricamento delle texture " << path << std::endl;
		textureId = -1;
	}
	SOIL_free_image_data(image);
	return textureId;
}

GLuint TextureLoader::LoadSkyboxMap(std::vector<const GLchar *> faces) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	int textureWidth, textureHeight;
	unsigned char* image;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	for (GLuint i = 0; i < faces.size(); i++) {
		image = SOIL_load_image(faces[i], &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureId;
}