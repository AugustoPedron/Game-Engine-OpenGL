#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/types.h>
#include "../../Texture/Texture.h"
#include "../../Shader/ShaderLoader.h"
#include "../../Strutture/MeshStruct.h"
#include "../../Strutture/SceneStruct.h"
#include "../../Camera/Camera.h"

class OctreeTester {
public:
	OctreeTester() {};
	OctreeTester(MeshStruct& Mesh);
	void updateTester(MeshStruct& Mesh);
	void DrawNoIndices(ShaderLoader* shader, Camera& camera);
	void DeleteMesh();
	inline GLuint getVAO() { return this->VAO; }
	inline std::vector<Vertex>& getVertices() { return this->vertices; }

private:
	std::vector<Vertex> vertices;
	GLuint VAO, VBO;
	void SetupMeshNoIndices();
};