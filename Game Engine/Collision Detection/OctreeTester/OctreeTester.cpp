#include "OctreeTester.h"

OctreeTester::OctreeTester(MeshStruct& Mesh) {
	this->vertices = Mesh.Vertices;
	this->SetupMeshNoIndices();
}

void OctreeTester::updateTester(MeshStruct& Mesh) {
	this->DeleteMesh();
	this->vertices = Mesh.Vertices;
	this->SetupMeshNoIndices();
}

void OctreeTester::DrawNoIndices(ShaderLoader* shaderLoader, Camera& camera) {
	shaderLoader->use();
	shaderLoader->SetMat4("projection", camera.getProjection());
	shaderLoader->SetMat4("view", camera.GetViewMatrix());
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINES, 0, this->vertices.size());
	glBindVertexArray(0);
}

void OctreeTester::SetupMeshNoIndices() {
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

	glBindVertexArray(0);
}

void OctreeTester::DeleteMesh() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	std::vector<Vertex>().swap(this->vertices);
}