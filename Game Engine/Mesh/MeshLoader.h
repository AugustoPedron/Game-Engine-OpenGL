#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/types.h>
#include "../Texture/Texture.h"
#include "../Shader/ShaderLoader.h"
#include "../Fisica/Shadows/Shadows.h"
#include "../Strutture/MeshStruct.h"

struct structTexture {
	GLuint id;
	std::string type;
	aiString path;
};

class MeshLoader {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<structTexture> textures;
	GLuint VAO, VBO, EBO;
	GLuint diffuseMap, specularMap;

	MeshLoader() {};
	MeshLoader(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<structTexture> textures);
	MeshLoader(std::vector<Vertex> vertices, const GLchar* texture_diffuse, const GLchar* texture_specular);
	void Draw(ShaderLoader shader);
	void DrawNoIndices(ShaderLoader shader);
	void DrawShadows(ShaderLoader shader, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix);
	void DeleteMesh();

private:

	//std::shared_ptr<ShadowSetter::Shadow> shadow;
	void SetupMesh();
	void SetupMeshNoIndices();
};