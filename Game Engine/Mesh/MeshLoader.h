#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/types.h>
#include "../Texture/Texture.h"
#include "../Shader/ShaderLoader.h"
#include "../Strutture/MeshStruct.h"

struct structTexture {
	GLuint id;
	std::string type;
	aiString path;
};

class MeshLoader {
public:
	MeshLoader() {};
	MeshLoader(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<structTexture> textures);
	MeshLoader(std::vector<Vertex> vertices, const GLchar* texture_diffuse, const GLchar* texture_specular);
	void Draw(ShaderLoader shader, unsigned int shadowMap);
	void DrawNoIndices(ShaderLoader shader, unsigned int shadowMap);
	void DeleteMesh();
	inline GLuint getVAO() { return this->VAO; }
	inline GLuint getDiffuseMap() { return this->diffuseMap; }
	inline std::vector<structTexture>& getTextures() { return this->textures; }
	inline std::vector<GLuint>& getIndices() { return this->indices; }

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<structTexture> textures;
	GLuint VAO, VBO, EBO;
	GLuint diffuseMap, specularMap;
	void SetupMesh();
	void SetupMeshNoIndices();
};