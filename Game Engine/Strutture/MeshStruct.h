#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct VertexOctree {
	glm::vec3 Position;
	glm::vec3 Normal;
};

struct IndexOctree {
	GLuint index;
	GLuint code;
	GLuint flag;
};

struct OctreeBounds {
	glm::vec3 position;
	GLfloat size;
};

struct OctreeOffset {
	GLfloat x;
	GLfloat y;
	GLfloat z;
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