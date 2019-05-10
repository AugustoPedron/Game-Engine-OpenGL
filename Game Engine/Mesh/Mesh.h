#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/types.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Texture/Texture.h"
#include "../Shader/Shader.h"
#include "../Fisica/Shadows/Shadows.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	GLuint VAO, VBO, EBO;
	GLuint diffuseMap, specularMap;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	Mesh(std::vector<Vertex> vertices,const GLchar* texture_diffuse, const GLchar* texture_specular);
	void Draw(Shader shader);
	void DrawNoIndices(Shader shader, glm::mat4 model);
	void DrawShadows(Shader shader, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix);

private:

	std::shared_ptr<ShadowSetter::Shadow> shadow;
	void SetupMesh();
	void SetupMeshNoIndices();
};