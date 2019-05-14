#pragma once
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct MeshStruct {
	std::vector<Vertex> Vertices;
	std::string texture_diffuse;
	std::string texture_specular;
};

struct SkyboxStruct {
	std::vector<GLfloat> Vertices;
	std::vector<const GLchar*> Textures;
};